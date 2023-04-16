// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Exceptions;
using AE.Platform;
using AE.Source;

using System.Diagnostics;

namespace AE.Projects;

public class CompileTree
{
    private readonly ProjectDirectory StandardDirectory;
    private readonly Dictionary<string, ModuleCompileItem> Items = new();
    private readonly Dictionary<string, CompileItem> DependSources = new();
    private readonly string ForcedIncludePath;

    public CompileTree(ProjectDirectory StandardDirectory)
    {
        this.StandardDirectory = StandardDirectory;
        ForcedIncludePath = Path.Combine(StandardDirectory.Source.Root, "Runtime", "Core", "Public", "CoreMinimal.h");
    }

    public void AddCompileItem(ModuleCompileItem Item)
    {
        lock (Items)
        {
            Items.Add(Item.SourceCode, Item);

            if (Item is ModuleCompileItem MCI)
            {
                DependSources.Add(MCI.InterfaceName ?? MCI.SourceCode, MCI);
            }
        }
    }

    private struct InterfaceRef
    {
        public required string? Name { get; set; }

        public required string Directory { get; set; }

        public required string Filename { get; set; }

        public required HeaderCompileItem.LookupMethod? LookupMethod { get; set; }
    }

    private readonly Dictionary<string, Task> SourceCodeCompileTasks = new();
    private readonly Dictionary<string, List<InterfaceRef>> DependenciesTree = new();

    public async Task CompileAsync(ModuleCompiler Compiler, CancellationToken SToken = default)
    {
        Console.WriteLine("Resolve dependencies...");
        foreach (var (_, Item) in DependSources.ToArray())
        {
            if (Item is ModuleCompileItem MCI)
            {
                foreach (var Required in MCI.RequiredModules)
                {
                    if (string.IsNullOrEmpty(Required.SourcePath) == false)
                    {
                        if (DependSources.ContainsKey(Required.Name) == false)
                        {
                            var CompileItem = new HeaderCompileItem(StandardDirectory, Required.Name, Required.SourcePath, Required.LookupMethod);
                            DependSources.Add(Required.Name, CompileItem);
                        }
                    }
                }
            }
        }

        Console.WriteLine("Execute {0} tasks with {1} threads.", DependSources.Count, Compiler.MaxParallelCompilers);

        List<Task> Tasks = new();
        foreach (var (_, Item) in DependSources)
        {
            Tasks.Add(CompileFileAsync(Compiler, Item, SToken));
        }

        await Task.WhenAll(Tasks);
    }

    private async Task CompileFileAsync(ModuleCompiler Compiler, CompileItem Item, CancellationToken SToken = default)
    {
        Task? PreviousTask;
        TaskCompletionSource? TCS = null;

        lock (SourceCodeCompileTasks)
        {
            if (SourceCodeCompileTasks.TryGetValue(Item.SourceCode, out PreviousTask) == false)
            {
                TCS = new();
                PreviousTask = TCS.Task;
                SourceCodeCompileTasks.Add(Item.SourceCode, PreviousTask);
            }
        }

        if (TCS != null)
        {
            try
            {
                string CompilerOptions = "/nologo /std:c++20 /c /EHsc";

                if (Item is ModuleCompileItem MCI)
                {
                    CompilerOptions += $" /Fo\"{Item.Output}\"";
                    CompilerOptions += $" {MCI.Options.Includes}";
                    CompilerOptions += $" {MCI.Options.Defines}";
                    CompilerOptions += $" /FI\"{ForcedIncludePath}\"";
                    if (MCI.InterfaceOutput != null)
                    {
                        CompilerOptions += $" /ifcOutput\"{MCI.InterfaceOutput}\"";
                    }

                    List<InterfaceRef> InterfaceRefs = new();
                    foreach (var Required in MCI.RequiredModules)
                    {
                        CompileItem DependItem = DependSources[Required.Name];
                        await CompileFileAsync(Compiler, DependItem, SToken);
                        lock (DependenciesTree)
                        {
                            InterfaceRefs.AddRange(DependenciesTree[Required.Name]);
                        }
                    }

                    foreach (var RefDirectory in InterfaceRefs.Select(p => p.Directory).Distinct())
                    {
                        CompilerOptions += $" /ifcSearchDir \"{RefDirectory}\"";
                    }
                    foreach (var (Name, Filename, LookupMethod) in InterfaceRefs.Select(p => (p.Name, p.Filename, p.LookupMethod)).Distinct())
                    {
                        if (LookupMethod.HasValue)
                        {
                            CompilerOptions += $" /headerUnit:{LookupMethod.Value.ToString().ToLower()} \"{Name}\"=\"{Filename}\"";
                        }
                    }

                    lock (DependenciesTree)
                    {
                        if (MCI.InterfaceName != null)
                        {
                            InterfaceRefs.Add(new InterfaceRef()
                            {
                                Name = MCI.InterfaceName,
                                Directory = Path.GetDirectoryName(MCI.InterfaceOutput)!,
                                Filename = Path.GetFileName(MCI.InterfaceOutput)!,
                                LookupMethod = null
                            });
                            DependenciesTree[MCI.InterfaceName] = InterfaceRefs;
                        }
                    }
                    string Output = await Compiler.RunCompilerAsync(Item.SourceCode, $"{Item.SourceCode} {CompilerOptions}", SToken);
                    Console.Write(Output);
                }
                else if (Item is HeaderCompileItem HCI)
                {
                    CompilerOptions += " /exportHeader";
                    switch (HCI.Method)
                    {
                        case HeaderCompileItem.LookupMethod.Angle:
                            CompilerOptions += $" /headerName:angle \"{HCI.SourceCode}\"";
                            lock (DependenciesTree)
                            {
                                DependenciesTree.Add(HCI.Name, new List<InterfaceRef>()
                                {
                                    new InterfaceRef()
                                    {
                                        Name = HCI.Name,
                                        Directory = Path.GetDirectoryName(HCI.InterfaceOutput)!,
                                        Filename = Path.GetFileName(HCI.InterfaceOutput)!,
                                        LookupMethod = HeaderCompileItem.LookupMethod.Angle
                                    }
                                });
                            }
                            break;
                        case HeaderCompileItem.LookupMethod.Quote:
                            CompilerOptions += $" /headerName:quote \"{HCI.SourceCode}\"";
                            lock (DependenciesTree)
                            {
                                DependenciesTree.Add(HCI.Name, new List<InterfaceRef>()
                                {
                                    new InterfaceRef()
                                    {
                                        Name = HCI.Name,
                                        Directory = Path.GetDirectoryName(HCI.InterfaceOutput)!,
                                        Filename = Path.GetFileName(HCI.InterfaceOutput)!,
                                        LookupMethod = HeaderCompileItem.LookupMethod.Quote
                                    }
                                });
                            }
                            break;
                    }
                    CompilerOptions += $" /Fo\"{HCI.Output}\"";
                    CompilerOptions += $" /ifcOutput\"{HCI.InterfaceOutput}\"";

                    string Output = await Compiler.RunCompilerAsync(Item.SourceCode, CompilerOptions, SToken);
                    Console.Write(Output);
                }

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
