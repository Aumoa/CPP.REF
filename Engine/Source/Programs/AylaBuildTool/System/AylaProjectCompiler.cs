// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.BuildSettings;
using AE.Compile;
using AE.Diagnostics;
using AE.Exceptions;
using AE.Platform;
using AE.Platform.Windows;
using AE.Projects;
using AE.Rules;
using AE.SourceTree;

using ModuleDependencyCache = AE.Projects.ModuleDependenciesResolver.ModuleDependencyCache;

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

    public async Task<int> CompileAsync(CancellationToken SToken = default)
    {
        // Spawn ToolChain.
        ToolChainInstallation ToolChain = SpawnPlatformToolChain();

        // Resolve dependencies.
        var Resolver = new ModuleDependenciesResolver(Rule, SearchedModules, ToolChain);
        Resolver.Resolve();

        Console.Write("Generate makefiles for {0}...", Target.TargetName);
        Dictionary<ModuleDependencyCache, Makefile> Makefiles;
        using (var Timer = new ScopedTimer("Generate Makefiles"))
        {
            Makefiles = await Resolver.GenerateMakefilesAsync(Rule, SToken);
        }
        Console.WriteLine(" {0} seconds elapsed.", ScopedTimer.GetElapsed("Generate Makefiles"));

        var CompileTasks = new CompileTasks(ToolChain);
        Dictionary<string, CompileNode> CompileTree;
        Dictionary<ModuleDependencyCache, (Task, TaskCompletionSource)> LinkerTree = new();

        {
            List<Task<(ModuleDependencyCache, string, CompileNode)?>> Tasks = new();

            foreach (var (ModuleCache, Makefile) in Makefiles)
            {
                foreach (var CompileItem in Makefile.CompileItems)
                {
                    Tasks.Add(CompileTasks.ScanDependenciesAsync(CompileItem, Rule, SToken).ContinueWith(p =>
                    {
                        MakefileCompile Item = p.Result;
                        if (Item.Provide != null)
                        {
                            return (ModuleCache, Item.Provide.InterfaceName, new CompileTreeNode
                            {
                                Compile = Item,
                                CompileTask = new(),
                                Parents = new(),
                                IfcSearchDirs = new() { Item.Cache.IntermediateOutput },
                                HeaderImports = new()
                            });
                        }
                        else
                        {
                            if (!Global.IsHeaderFile(p.Result.SourceCode))
                            {
                                return (ModuleCache, Item.SourceCode, new CompileNode() { Compile = Item, IfcSearchDirs = new(), HeaderImports = new(), Parents = new() });
                            }
                            else
                            {
                                return ((ModuleDependencyCache, string, CompileNode)?)null;
                            }
                        }
                    }));
                }
            }

            var CompileList = (await Task.WhenAll(Tasks)).Where(p => p != null).Select(p => p!.Value);
            CompileTree = CompileList.ToDictionary(p => p.Item2, p => p.Item3);

            Dictionary<ModuleDependencyCache, List<CompileNode>> LinkCompiles = new();
            foreach (var (ModuleCache, _, CompileItem) in CompileList)
            {
                if (LinkCompiles.TryGetValue(ModuleCache, out List<CompileNode>? CompileNodes) == false)
                {
                    CompileNodes = new();
                    LinkCompiles.Add(ModuleCache, CompileNodes);
                }
                CompileNodes.Add(CompileItem);
            }

            var CurrentLinkerTask = LinkCompiles.Select(p =>
            {
                List<Task> CompilerTasks = new();
                foreach (var Node in p.Value)
                {
                    if (Node is CompileTreeNode TreeNode)
                    {
                        CompilerTasks.Add(TreeNode.CompileTask.Task);
                    }
                }

                Task CurrentTask = Task.WhenAll(CompilerTasks);
                return (p.Key, CurrentTask);
            }).ToDictionary(p => p.Key, p => p.CurrentTask);

            (Task, TaskCompletionSource) ResolveDependencyLinkTree(ModuleDependencyCache ModuleCache)
            {
                if (LinkerTree.TryGetValue(ModuleCache, out var FullTask))
                {
                    return FullTask;
                }

                List<Task> DependTasks = new();
                foreach (var Depend in ModuleCache.DependModules)
                {
                    ModuleDependencyCache DependModuleCache = Resolver.GetDependencyCache(Depend);
                    DependTasks.Add(ResolveDependencyLinkTree(DependModuleCache).Item1);
                }

                FullTask.Item1 = Task.WhenAll(DependTasks.Append(CurrentLinkerTask[ModuleCache]));
                FullTask.Item2 = new TaskCompletionSource();
                LinkerTree.Add(ModuleCache, FullTask);
                return FullTask;
            }

            foreach (var (ModuleCache, _) in CurrentLinkerTask)
            {
                _ = ResolveDependencyLinkTree(ModuleCache);
            }
        }

        HashSet<CompileNode> Resolved = new();
        void ResolveCompileTreeNode(CompileNode Node)
        {
            if (Resolved.Add(Node) == false)
            {
                return;
            }

            foreach (var Depend in Node.Compile.Requires)
            {
                if (CompileTree.TryGetValue(Depend.InterfaceName, out CompileNode? SubNode) == false)
                {
                    if (Depend.SourcePath != null)
                    {
                        SubNode = new CompileTreeNode()
                        {
                            Compile = Node.Compile with
                            {
                                SourceCode = Depend.SourcePath,
                                Cache = new MakefileCache()
                                {
                                    SourceCodePath = Depend.SourcePath,
                                    SourceCodeHash = Global.GenerateSourceCodeHash(Depend.SourcePath),
                                    LastWriteTimeInUtc = File.GetLastWriteTimeUtc(Depend.SourcePath),
                                    Dependencies = Array.Empty<string>(),
                                    Includes = Array.Empty<string>(),
                                    IntermediateOutput = Node.Compile.Cache.IntermediateOutput,
                                    ObjectOutput = string.Empty,
                                    InterfaceOutput = string.Empty
                                },
                                Provide = Depend,
                                Requires = Array.Empty<CppModuleDescriptor>(),
                            },
                            CompileTask = new(),
                            Parents = new(),
                            IfcSearchDirs = new(),
                            HeaderImports = new()
                        };

                        CompileTree.Add(Depend.InterfaceName, SubNode);
                    }
                }

                if (SubNode == null)
                {
                    continue;
                }

                // Resolve sub tree node recursive.
                ResolveCompileTreeNode(SubNode);

                Node.Parents.Add(SubNode);

                foreach (var IfcSearchDir in SubNode.IfcSearchDirs)
                {
                    Node.IfcSearchDirs.Add(IfcSearchDir);
                }
                foreach (var HeaderImport in SubNode.HeaderImports)
                {
                    Node.HeaderImports.Add(HeaderImport);
                }
                if (Depend.LookupMethod.HasValue)
                {
                    Node.HeaderImports.Add(Depend.InterfaceName);
                }
            }
        }

        foreach (var Node in CompileTree.Values.ToArray())
        {
            ResolveCompileTreeNode(Node);
        }

        // Compile module sources.
        {
            List<Task> Tasks = new();
            foreach (var (_, Node) in CompileTree)
            {
                async Task<string?> CompileNodeAsync(CancellationToken SToken)
                {
                    CompileTreeNode? TreeNode = Node as CompileTreeNode;

                    try
                    {
                        foreach (var SubNode in Node.Parents)
                        {
                            if (SubNode is CompileTreeNode SubTreeNode)
                            {
                                await SubTreeNode.CompileTask.Task;
                            }
                        }

                        string Result = await CompileTasks.CompileAsync(Node, Rule, SToken);
                        TreeNode?.CompileTask.SetResult();
                        Console.WriteLine(Result);
                        return Result;
                    }
                    catch (Exception E)
                    {
                        if (E is TaskCanceledException)
                        {
                            TreeNode?.CompileTask.SetCanceled();
                        }
                        else if (E is TerminateException TE)
                        {
                            Console.Error.WriteLine(TE.Message);
                        }
                        else
                        {
                            TreeNode?.CompileTask.SetException(E);
                        }
                        return null;
                    }
                }

                Tasks.Add(CompileNodeAsync(SToken));
            }

            foreach (var (ModuleCache, (CompileTask, LinkerTCS)) in LinkerTree)
            {
                async Task Link()
                {
                    try
                    {
                        await CompileTask;

                        List<Task> DependTasks = new();
                        foreach (string Depend in ModuleCache.DependModules)
                        {
                            ModuleDependencyCache DependCache = Resolver.GetDependencyCache(Depend);
                            if (LinkerTree.TryGetValue(DependCache, out var OutputTasks))
                            {
                                DependTasks.Add(OutputTasks.Item2.Task);
                            }
                        }
                        await Task.WhenAll(DependTasks);

                        Linker Linker = ToolChain.SpawnLinker();
                        await Linker.LinkAsync(Rule, ModuleCache);

                        LinkerTCS.SetResult();
                    }
                    catch (Exception E)
                    {
                        LinkerTCS.SetException(E);
                    }
                }
                Tasks.Add(Link());
            }

            await Task.WhenAll(Tasks);
        }

        return 0;
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

    private ToolChainInstallation SpawnPlatformToolChain()
    {
        if (TargetInfo.BuildConfiguration.Platform == TargetPlatform.Win64)
        {
            var Installations = VisualStudioInstallation.FindVisualStudioInstallations(CompilerVersion.VisualStudio2022);
            if (Installations.Any())
            {
                return Installations[0];
            }
        }
        throw new TerminateException(KnownErrorCode.PlatformCompilerNotFound, CoreStrings.Errors.PlatformCompilerNotFound(TargetInfo.BuildConfiguration.Platform.ToString()));
    }
}
