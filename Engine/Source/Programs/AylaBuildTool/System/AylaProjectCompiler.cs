// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Data;
using System.Diagnostics;

using AE.AppHelper;
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

        await GenerateReflectionCodesAsync(Resolver, InCancellationToken);

        int ReturnCode = await CompileShadersAsync(ToolChain, Rule.Target.BuildConfiguration.Platform.Architecture, Resolver, InCancellationToken);
        if (ReturnCode != 0)
        {
            return ReturnCode;
        }

        Console.Write("Generate makefiles for {0}...", Target.TargetName);
        Dictionary<ModuleInformation, Makefile> Makefiles;
        using (var Timer = new ScopedTimer("Generate Makefiles"))
        {
            Makefiles = await Resolver.GenerateMakefilesAsync(ToolChain, Rule, InCancellationToken);
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
                Context.CompileNodes.Add(Makefile, new());
                foreach (var Item in Makefile.CompileItems)
                {
                    if (Item.Cache != null)
                    {
                        continue;
                    }

                    Context.CompileNodes[Makefile].Add(new CompileNode()
                    {
                        Compile = Item
                    });
                }
            }

            ReturnCode = await DispatchCompileWorkers(ToolChain, Context, Rule, InCancellationToken);
            if (ReturnCode != 0)
            {
                return ReturnCode;
            }

            string DotNETPath = ToolChain.DotNET;
            foreach (var (ModuleInfo, _) in Makefiles)
            {
                string ScriptPath = Path.Combine(ModuleInfo.ScriptPath, ModuleInfo.Name + ".CSharp.csproj");
                if (File.Exists(ScriptPath))
                {
                    var Config = Rule.Target.BuildConfiguration.Configuration.IsDebug() ? "Debug" : "Release";
                    var PSI = new ProcessStartInfo();
                    PSI.FileName = DotNETPath;
                    PSI.Arguments = $"build -c {Config} -v minimal \"{ScriptPath}\"";
                    var Builder = await App.Run(PSI, true, InCancellationToken);
                    if (Builder.ExitCode != 0)
                    {
                        return Builder.ExitCode;
                    }
                }
            }

            return 0;
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
        ATarget? Target = InWorkspace.SearchTargetByName(InTargetName);
        if (Target == null && InTargetName.EndsWith("Editor"))
        {
            Target = InWorkspace.SearchTargetByName(InTargetName.Replace("Editor", string.Empty));
        }
        if (Target == null)
        {
            throw new TerminateException(KnownErrorCode.TargetNotFound, CoreStrings.Errors.TargetNotFoundException(InTargetName));
        }
        await Target.ConfigureAsync(SToken);

        TargetRules Rule = Target.GenerateTargetRule(InTargetInfo);
        Dictionary<string, SearchedModule> SearchedModules = new();
        InWorkspace.SearchCXXModulesRecursive(Rule, SearchedModules, Rule.Name, Rule.TargetModuleName);
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

        public required List<Task<bool>> Compiles { get; init; }

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
            ModuleInformation ModuleInfo = Makefile.ModuleInfo;

            foreach (var Node in Nodes)
            {
                Debug.Assert(ModuleInfo == Node.Compile.ModuleInfo);

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

            LinkModules.Add(ModuleInfo.Name, new() { Compiles = ScopedTasks, ModuleInfo = ModuleInfo });
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
                Linker Linker = ToolChain.SpawnLinker();
                if (LinkContext.Compiles.Any())
                {
                    bool bCompileResult = await Task.WhenAll(LinkContext.Compiles).ContinueWith(p => !p.Result.Any(p => !p));
                    if (bCompileResult == false)
                    {
                        // One or more compile tasks are failed.
                        LinkContext.LinkCompleted.SetResult(false);
                        return;
                    }
                }
                else
                {
                    bool bCached = await Linker.TryCheckOutputsAsync(Rule, LinkContext.ModuleInfo, InCancellationToken);
                    if (bCached)
                    {
                        // All compile items and output items is cached.
                        LinkContext.LinkCompleted.SetResult(true);
                        return;
                    }
                }

                bool[] DependLinkResults = await Task.WhenAll(DependTasks);
                if (DependLinkResults.Any(p => !p))
                {
                    // One or more depend links are failed.
                    LinkContext.LinkCompleted.SetResult(false);
                    return;
                }

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

    private async Task GenerateReflectionCodesAsync(ModuleDependenciesResolver Resolver, CancellationToken InCancellationToken)
    {
        Console.WriteLine("Parsing headers for {0}", Target.TargetName);
        using (var Timer = new ScopedTimer("Parsing Headers"))
        {
            await Resolver.GenerateReflectionCodesAsync(InCancellationToken);
        }
        Console.WriteLine("Reflection code generated in {0} seconds.", ScopedTimer.GetElapsed("Parsing Headers"));
    }

    private async Task<int> CompileShadersAsync(ToolChainInstallation ToolChain, Architecture TargetArchitecture, ModuleDependenciesResolver Resolver, CancellationToken InCancellationToken)
    {
        if (Resolver.HasDependModule("RHI") == false)
        {
            return 0;
        }

        Process? P;
        using (var Timer = new ScopedTimer("Compile Shaders"))
        {
            var ShaderSourceDirectory = Workspace.CurrentTarget.Shaders;
            var ShaderOutputDirectory = Workspace.CurrentTarget.Intermediate.Shaders;

            var PSI = new ProcessStartInfo();
            if (Environment.OSVersion.Platform == PlatformID.Unix)
            {
                string BaseDir = Path.Combine(Global.EngineDirectory.Binaries.Linux, "Shipping");
                PSI.Environment["LD_LIBRARY_PATH"] = BaseDir;
                PSI.FileName = Path.Combine(BaseDir, "ShaderCompileWorker");
            }
            else if (Environment.OSVersion.Platform == PlatformID.Win32NT)
            {
                PSI.FileName = Global.EngineDirectory.Binaries.Win64;
                PSI.FileName = Path.Combine(PSI.FileName, "Shipping", "ShaderCompileWorker.exe");
            }
            else
            {
                throw new TerminateException(KnownErrorCode.NotSupportedBuildHostPlatform, CoreStrings.Errors.NotSupportedBuildHostPlatform);
            }

            PSI.WorkingDirectory = ToolChain.GetShaderCompilerDirectory(TargetArchitecture);
            PSI.Arguments = $"-Input \"{ShaderSourceDirectory}\" -Output \"{ShaderOutputDirectory}\"";
            Console.WriteLine("Running ShaderCompileWorker \"{0}\"", Workspace.CurrentTarget.Name);
            P = Process.Start(PSI);
            Debug.Assert(P != null);
            await P.WaitForExitAsync(InCancellationToken);
            P.WaitForExit();
        }
        Console.WriteLine("Shader code generated in {0} seconds.", ScopedTimer.GetElapsed("Compile Shaders"));
        return P.ExitCode;
    }
}
