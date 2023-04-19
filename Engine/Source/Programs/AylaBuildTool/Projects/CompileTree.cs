// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Text.Json.Nodes;

using AE.BuildSettings;
using AE.Platform;
using AE.System;

namespace AE.Projects;

public class CompileTree
{
    private readonly string ForcedIncludePath;
    private readonly ToolChainInstallation ToolChain;
    private readonly ModuleDependenciesResolver Resolver;
    private readonly string[] PlatformMacros;

    public CompileTree(ModuleDependenciesResolver Resolver, ToolChainInstallation ToolChain)
    {
        this.ForcedIncludePath = Path.Combine(Global.EngineDirectory.Source.Root, "Runtime", "Core", "Public", "CoreMinimal.h");
        this.Resolver = Resolver;
        this.ToolChain = ToolChain;

        var CurrentBuildConfig = Resolver.Rule.Target.BuildConfiguration;
        HashSet<string> PlatformMacros = new();
        PlatformGroup.ForEach(p =>
        {
            bool bCurrent = p == CurrentBuildConfig.Platform.Group;
            PlatformMacros.Add($"PLATFORM_{p.GroupName.ToUpper()}={(bCurrent ? 1 : 0)}");
        });
        this.PlatformMacros = PlatformMacros.ToArray();
    }

    private readonly Dictionary<string, CompileItem> InterfaceTree = new();

    private string GenerateBaseCompilerOptions(ModuleDependenciesResolver.ModuleDependencyCache? DependencyCache, string? OutputDirectory, params CompilerOptions[] AdditionalArgs)
    {
        CompilerOptions[] Options = new[]
        {
            CompilerOptions.NoLogo,
            CompilerOptions.CppStandard,
            CompilerOptions.Compile,
            CompilerOptions.EnableExceptions
        };

        IEnumerable<string> Args = Options.Concat(AdditionalArgs).Select(p => ToolChain.GetCompilerOptions(p));
        if (DependencyCache?.IncludePaths.Any() == true)
        {
            Args = Args.Append(ToolChain.GetCompilerOptions(CompilerOptions.Includes, (object?[])DependencyCache.IncludePaths));
        }
        if (DependencyCache?.AdditionalMacros.Any() == true)
        {
            Args = Args.Append(ToolChain.GetCompilerOptions(CompilerOptions.Defines, (object?[])DependencyCache.AdditionalMacros));
        }
        if (OutputDirectory != null)
        {
            Args = Args.Append(ToolChain.GetCompilerOptions(CompilerOptions.InterfaceOutputDirectory, OutputDirectory));
        }

        return string.Join(' ', Args);
    }

    public async Task ScanDependenciesAsync(CancellationToken SToken = default)
    {
        List<Task> Tasks = new();
        var Compiler = new ModuleCompiler(ToolChain, ToolChain.GetPrimaryCompilerName());

        Console.WriteLine("Scan dependencies...");

        foreach (var Rule in Resolver.GetModules())
        {
            var DependencyCache = Resolver.GetDependencyCache(Rule.Name);

            string IntermediateBuild = DependencyCache.ProjectDir.Intermediate.Build;
            var BuildConfiguration = Resolver.Rule.Target.BuildConfiguration.Configuration;
            string OutputDirectory = Path.Combine(IntermediateBuild, BuildConfiguration.ToString(), Rule.Name);

            string CompilerArgs = GenerateBaseCompilerOptions(DependencyCache, OutputDirectory, CompilerOptions.ScanDependencies);
            HashSet<Exception> Exceptions = new();

            foreach (var SourceFile in DependencyCache.SourceFiles.Where(Global.IsSourceCode))
            {
                string OutputFilename = Path.Combine(OutputDirectory, Path.ChangeExtension(Path.GetFileName(SourceFile), ".obj"));
                string AdditionalArgs = $" {ToolChain.GetCompilerOptions(CompilerOptions.Output, OutputFilename)} {SourceFile}";
                Tasks.Add(Compiler.RunCompilerAsync(SourceFile, CompilerArgs + AdditionalArgs, SToken).ContinueWith(p =>
                {
                    if (p.IsFaulted)
                    {
                        lock (Exceptions)
                        {
                            Exceptions.Add(p.Exception!);
                        }
                    }
                    else
                    {
                        JsonNode? DependenciesTree = JsonNode.Parse(p.Result);
                        var CompileItem = new ModuleCompileItem(SourceFile, DependenciesTree, Resolver.GetDependencyCache(Rule.Name));

                        lock (InterfaceTree)
                        {
                            InterfaceTree.Add(CompileItem.LogicalName, CompileItem);
                        }
                    }
                }));
            }

            if (Exceptions.Any())
            {
                throw new AggregateException(Exceptions.ToArray());
            }
        }

        await Task.WhenAll(Tasks);

        HashSet<string> DirectoryCache = new();
        void CreateBaseDirectory(string? Filename)
        {
            if (Filename == null)
            {
                return;
            }

            string Dir = Path.GetDirectoryName(Filename)!;
            if (DirectoryCache.Add(Dir))
            {
                Directory.CreateDirectory(Dir);
            }
        }

        foreach (var (LogicalName, CompileItem) in InterfaceTree.ToArray())
        {
            foreach (var Depend in CompileItem.Dependencies)
            {
                CreateBaseDirectory(CompileItem.Output);
                CreateBaseDirectory(CompileItem.InterfaceOutput);

                if (InterfaceTree.ContainsKey(Depend.LogicalName) == false)
                {
                    if (Depend is CompileItem.HeaderDependencyItem HDI)
                    {
                        HeaderCompileItem NewItem = new(HDI.LogicalName, HDI.SourceCode, HDI.LookupMethod);
                        CreateBaseDirectory(NewItem.Output);
                        CreateBaseDirectory(NewItem.InterfaceOutput);
                        InterfaceTree.Add(NewItem.LogicalName, NewItem);
                    }
                    else
                    {
                        Debug.Assert(false);
                    }
                }
            }
        }
    }

    public async Task CompileAsync(CancellationToken SToken = default)
    {
        List<Task> Tasks = new();
        var Compiler = new ModuleCompiler(ToolChain, ToolChain.GetPrimaryCompilerName());

        Console.WriteLine("Compile {0} items with {1} compiler threads.", InterfaceTree.Count, Compiler.MaxParallelCompilers);

        int ParallelNumber = 0;
        Dictionary<string, Task> SourceCodeCompileTasks = new();

        HashSet<Exception> Exceptions = new();
        foreach (var (LogicalName, Item) in InterfaceTree)
        {
            Tasks.Add(CompileFileAsync(Compiler, SourceCodeCompileTasks, Item, Output =>
            {
                int Current = Interlocked.Increment(ref ParallelNumber);
                Console.Write("[{0}/{1}] {2}", Current, InterfaceTree.Count, Output);
            }, SToken).ContinueWith(p =>
            {
                if (p.IsFaulted)
                {
                    if (p.Exception!.InnerExceptions.Count > 1)
                    {
                        Exceptions.Add(p.Exception);
                    }
                    else
                    {
                        Exceptions.Add(p.Exception.InnerException ?? p.Exception.InnerExceptions.First());
                    }
                }
            }, SToken));
        }

        await Task.WhenAll(Tasks);
        if (Exceptions.Any())
        {
            throw new AggregateException(Exceptions.ToArray());
        }
    }

    private struct InterfaceRef
    {
        public required string LogicalName { get; set; }

        public required string? Directory { get; set; }

        public required string Filename { get; set; }

        public required HeaderCompileItem.LookupMethod? LookupMethod { get; set; }
    }

    private readonly Dictionary<string, List<InterfaceRef>> CompiledInterfaces = new();

    private async Task CompileFileAsync(ModuleCompiler Compiler, Dictionary<string, Task> SourceCodeCompileTasks, CompileItem Item, Action<string> Report, CancellationToken SToken = default)
    {
        Task? PreviousTask;
        TaskCompletionSource? TCS = null;

        lock (SourceCodeCompileTasks)
        {
            if (SourceCodeCompileTasks.TryGetValue(Item.LogicalName, out PreviousTask) == false)
            {
                TCS = new();
                PreviousTask = TCS.Task;
                SourceCodeCompileTasks.Add(Item.LogicalName, PreviousTask);
            }
        }

        if (TCS != null)
        {
            try
            {
                string CompilerArgs = GenerateBaseCompilerOptions(null, null);
                CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.Output, Item.Output)}";

                if (Item.InterfaceOutput != null)
                {
                    CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.InterfaceOutput, Item.InterfaceOutput)}";
                }

                string Output;

                if (Item is ModuleCompileItem MCI)
                {
                    var DCache = MCI.DependencyCache;
                    CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.Includes, (object?[])DCache.IncludePaths)}";
                    CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.Defines, (object?[])DCache.AdditionalMacros)}";
                    CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.ForceInclude, ForcedIncludePath)}";

                    var ApiDescriptions = DCache.ApiDescriptions.Select(p => $"{p}={ToolChain.GetSharedSymbol(false)}");
                    ApiDescriptions = ApiDescriptions.Append($"{DCache.Name.ToUpper()}_API={ToolChain.GetSharedSymbol(true)}");
                    CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.Defines, (object?[])ApiDescriptions.ToArray())}";

                    CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.Defines, (object?[])PlatformMacros)}";

                    List<InterfaceRef> InterfaceRefs = new();
                    foreach (var Depend in MCI.Dependencies)
                    {
                        CompileItem DependItem = InterfaceTree[Depend.LogicalName];
                        await CompileFileAsync(Compiler, SourceCodeCompileTasks, DependItem, Report, SToken);
                        lock (CompiledInterfaces)
                        {
                            if (DependItem is HeaderCompileItem HCI)
                            {
                                InterfaceRefs.Add(new InterfaceRef()
                                {
                                    LogicalName = DependItem.LogicalName,
                                    Directory = null,
                                    Filename = DependItem.InterfaceOutput!,
                                    LookupMethod = ((CompileItem.HeaderDependencyItem)Depend).LookupMethod
                                });
                            }
                            else
                            {
                                InterfaceRefs.AddRange(CompiledInterfaces[Depend.LogicalName]);
                            }
                        }
                    }

                    // Add search directories for modules.
                    CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.InterfaceSearchDir, (object?[])InterfaceRefs.Where(p => p.Directory != null).Select(p => p.Directory).Distinct().ToArray())}";

                    // Add reference for header files.
                    foreach (var (Name, Filename, LookupMethod) in InterfaceRefs.Where(p => p.LookupMethod != null).Select(p => (p.LogicalName, p.Filename, p.LookupMethod)).Distinct())
                    {
                        CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.HeaderUnit, LookupMethod!.Value, Name, Filename)}";
                    }

                    lock (CompiledInterfaces)
                    {
                        if (MCI.InterfaceOutput != null)
                        {
                            InterfaceRefs.Add(new InterfaceRef()
                            {
                                LogicalName = MCI.LogicalName,
                                Directory = Path.GetDirectoryName(MCI.InterfaceOutput)!,
                                Filename = Path.GetFileName(MCI.InterfaceOutput)!,
                                LookupMethod = null
                            });
                            CompiledInterfaces[MCI.LogicalName] = InterfaceRefs;
                        }
                    }

                    Output = await Compiler.RunCompilerAsync(Item.SourceCode, $"\"{Item.SourceCode}\" {CompilerArgs}", SToken);
                }
                else if (Item is HeaderCompileItem HCI)
                {
                    CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.ExportHeader)}";
                    CompilerArgs += $" {ToolChain.GetCompilerOptions(CompilerOptions.HeaderReference, HCI.Method, HCI.SourceCode)}";
                    Output = await Compiler.RunCompilerAsync(Item.SourceCode, $"\"{Item.SourceCode}\" {CompilerArgs}", SToken);
                }
                else
                {
                    throw new InvalidOperationException("Internal error.");
                }

                Report(Output);
                TCS.SetResult();
            }
            catch (Exception E)
            {
                TCS.SetException(E);
            }
        }

        await PreviousTask;
    }
}
