// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.BuildSettings;
using AE.Diagnostics;
using AE.Exceptions;
using AE.Platform;
using AE.Platform.Linux;
using AE.Platform.Windows;
using AE.Projects;
using AE.Rules;
using AE.SourceTree;

namespace AE.System;

public class AylaProjectCompiler
{
    public required Workspace Workspace { get; init; }

    public required ATarget Target { get; init; }

    public required TargetInfo TargetInfo { get; init; }

    public required TargetRules Rule { get; init; }

    private readonly Dictionary<string, SearchedModule> SearchedModules;

    private AylaProjectCompiler(Dictionary<string, SearchedModule> InSearchedModules)
    {
        SearchedModules = InSearchedModules;
    }

    public async Task<int> CompileAsync(CancellationToken InCancellationToken = default)
    {
        // Spawn ToolChain.
        ToolChainInstallation ToolChain = SpawnPlatformToolChain();

        // Resolve dependencies.
        var Resolver = new ModuleDependenciesResolver(Rule, SearchedModules, ToolChain);
        Resolver.Resolve();

        Console.Write("Generate makefiles for {0}...", Target.TargetName);
        Dictionary<ModuleInformation, Makefile> Makefiles;
        using (var Timer = new ScopedTimer("Generate Makefiles"))
        {
            Makefiles = await Resolver.GenerateMakefilesAsync(Rule, InCancellationToken);
        }
        Console.WriteLine(" {0} seconds elapsed.", ScopedTimer.GetElapsed("Generate Makefiles"));

        try
        {
            var Context = new MakefileContext()
            {
                CompileNodes = new()
            };
            
            foreach (var (ModuleInfo, Makefile) in Makefiles)
            {
                foreach (var Item in Makefile.CompileItems)
                {
                    if (Item.Cache != null)
                    {
                        continue;
                    }

                    if (Context.CompileNodes.TryGetValue(Makefile, out var List) == false)
                    {
                        List = new();
                        Context.CompileNodes.Add(Makefile, List);
                    }

                    List.Add(new CompileNode()
                    {
                        Compile = Item
                    });
                }
            }

            int ReturnCode = await DispatchCompileWorkers(ToolChain, Context, Rule, InCancellationToken);
            return ReturnCode;
        }
        finally
        {
            List<Task> Tasks = new();
            foreach (var (_, Makefile) in Makefiles)
            {
                Tasks.Add(Makefile.SaveMakefileCacheAsync());
            }
            await Task.WhenAll(Tasks);
        }
    }

    public static async Task<AylaProjectCompiler> CreateCompilerAsync(Workspace InWorkspace, string InTargetName, TargetInfo InTargetInfo, CancellationToken SToken = default)
    {
        ATarget Target = InWorkspace.SearchTargetByName(InTargetName)
            ?? throw new TerminateException(KnownErrorCode.TargetNotFound, CoreStrings.Errors.TargetNotFoundException(InTargetName));
        await Target.ConfigureAsync(SToken);

        TargetRules Rule = Target.GenerateTargetRule(InTargetInfo);
        Dictionary<string, SearchedModule> SearchedModules = new();
        Global.SearchCXXModulesRecursive(InWorkspace, Rule, SearchedModules, Rule.Name, Rule.TargetModuleName);
        return new AylaProjectCompiler(SearchedModules)
        {
            Workspace = InWorkspace,
            Target = Target,
            TargetInfo = InTargetInfo,
            Rule = Rule,
        };
    }

    private record MakefileContext
    {
        public required Dictionary<Makefile, List<CompileNode>> CompileNodes { get; init; }

        public int TotalCount => CompileNodes.Select(p => p.Value.Count).Sum();
    }

    private record ModuleLinkContext
    {
        public required ModuleInformation ModuleInfo { get; init; }

        public required Task<bool> Compiles { get; init; }

        public TaskCompletionSource<bool> LinkCompleted { get; } = new();
    }

    private async Task<int> DispatchCompileWorkers(ToolChainInstallation ToolChain, MakefileContext Context, TargetRules Rule, CancellationToken InCancellationToken)
    {
        List<Task> Tasks = new();
        var CompileTasks = new CompileTasks(ToolChain);
        int ReturnCode = 0;

        string DIAG_TotalString = Context.TotalCount.ToString();
        int DIAG_CompileCounter = 0;
        Dictionary<string, ModuleLinkContext> LinkModules = new();

        foreach (var (Makefile, Nodes) in Context.CompileNodes)
        {
            List<Task<bool>> ScopedTasks = new();
            ModuleInformation? ModuleInfo = null;

            foreach (var Node in Nodes)
            {
                Debug.Assert(ModuleInfo == null || ModuleInfo == Node.Compile.ModuleInfo);
                ModuleInfo = Node.Compile.ModuleInfo;

                ScopedTasks.Add(CompileTasks.CompileAsync(Node, Rule, InCancellationToken).ContinueWith(p =>
                {
                    if (p.IsCanceled || p.IsFaulted)
                    {
                        if (p.Exception?.InnerException is TerminateException TE)
                        {
                            Console.Error.WriteLine(TE.Message);
                            Interlocked.CompareExchange(ref ReturnCode, (int)TE.ErrorCode, 0);
                        }
                        else
                        {
                            Console.Error.WriteLine("Internal compiler error: {0}", p.Exception);
                            Interlocked.CompareExchange(ref ReturnCode, -1, 0);
                        }

                        return false;
                    }
                    else
                    {
                        string MyCount = Interlocked.Increment(ref DIAG_CompileCounter).ToString().PadLeft(DIAG_TotalString.Length);
                        Console.WriteLine("[{0}/{1}] {2}", MyCount, DIAG_TotalString, p.Result);
                        return true;
                    }
                }));
            }

            if (ModuleInfo != null)
            {
                Task<bool> ContinuationTask = Task.WhenAll(ScopedTasks).ContinueWith(p => !p.Result.Any(p => !p));
                LinkModules.Add(ModuleInfo.Name, new() { Compiles = ContinuationTask, ModuleInfo = ModuleInfo });
            }
        }

        foreach (var (_, LinkContext) in LinkModules)
        {
            List<Task<bool>> DependTasks = new();
            foreach (var DependModuleName in LinkContext.ModuleInfo.DependModules)
            {
                if (LinkModules.TryGetValue(DependModuleName, out var DependContext))
                {
                    DependTasks.Add(DependContext.LinkCompleted.Task);
                }
            }

            async Task ContinuationAsync()
            {
                bool bCompileResult = await LinkContext.Compiles;
                if (bCompileResult == false)
                {
                    // One or more compile tasks are failed.
                    LinkContext.LinkCompleted.SetResult(false);
                    return;
                }

                bool[] DependLinkResults = await Task.WhenAll(DependTasks);
                if (DependLinkResults.Any(p => !p))
                {
                    // One or more depend links are failed.
                    LinkContext.LinkCompleted.SetResult(false);
                    return;
                }

                Linker Linker = ToolChain.SpawnLinker();
                try
                {
                    string Output = await Linker.LinkAsync(Rule, LinkContext.ModuleInfo, InCancellationToken);
                    LinkContext.LinkCompleted.SetResult(true);
                    Console.WriteLine(Output);
                }
                catch (Exception E)
                {
                    LinkContext.LinkCompleted.SetResult(false);
                    Console.Error.WriteLine("Link error: {0}", E.Message);
                    Interlocked.CompareExchange(ref ReturnCode, -1, 0);
                }
            }

            Tasks.Add(ContinuationAsync());
        }

        await Task.WhenAll(Tasks);
        return ReturnCode;
    }

    private ToolChainInstallation SpawnPlatformToolChain()
    {
        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            // Host:Windows Target:Windows
            if (TargetInfo.BuildConfiguration.Platform == TargetPlatform.Win64)
            {
                var Installations = VisualStudioInstallation.FindVisualStudioInstallations(CompilerVersion.VisualStudio2022);
                if (Installations.Any())
                {
                    return Installations[0];
                }
            }
        }
        else if (Environment.OSVersion.Platform == PlatformID.Unix)
        {
            // Host:Linux Target:Linux
            if (TargetInfo.BuildConfiguration.Platform == TargetPlatform.Linux)
            {
                return new LinuxToolChain();
            }
        }

        throw new TerminateException(KnownErrorCode.PlatformCompilerNotFound, CoreStrings.Errors.PlatformCompilerNotFound(TargetInfo.BuildConfiguration.Platform.ToString()));
    }
}
