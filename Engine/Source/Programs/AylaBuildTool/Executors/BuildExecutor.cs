// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.CLI;
using AE.Exceptions;
using AE.Misc;
using AE.Platform;
using AE.Platform.Windows;
using AE.Projects;
using AE.Rules;
using AE.SourceTree;
using AE.System;

namespace AE.Executors;

public class BuildExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "Target", IsRequired = true)]
        public string Target { get; set; } = null!;

        [CommandLineApply(CategoryName = "Clean")]
        public bool bClean { get; set; }
    }

    private readonly Arguments BuildArgs = new();

    public BuildExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(BuildArgs);
    }

    public async Task<int> RunAsync(CancellationToken SToken = default)
    {
        if (BuildArgs.bClean == true)
        {
            foreach (var Dir in GetProjectDirectories())
            {
                Dir.Cleanup();
            }
        }

        Workspace Workspace = await ConfigureWorkspaceAsync(SToken);

        ATarget? CurrentTarget = Workspace.SearchTargetByName(BuildArgs.Target);
        if (CurrentTarget == null)
        {
            throw new TerminateException(6, CoreStrings.Errors.TargetNotFoundException, BuildArgs.Target);
        }
        await CurrentTarget.ConfigureAsync(SToken);

        var TargetInfo = new TargetInfo()
        {
            BuildConfiguration = new()
            {
                Configuration = Configuration.Shipping,
                Platform = TargetPlatform.Win64
            }
        };
        TargetRules TargetRule = CurrentTarget.GenerateTargetRule(TargetInfo);
        Dictionary<string, SearchedModule> SearchedModules = new();
        Global.SearchCXXModulesRecursive(Workspace, TargetRule, SearchedModules, TargetRule.Name, TargetRule.TargetModuleName);

        var Installations = VisualStudioInstallation.FindVisualStudioInstallations(Platform.Windows.CompilerVersion.VisualStudio2022);
        if (Installations.Any() == false)
        {
            throw new InvalidOperationException("Internal error.");
        }

        ToolChainInstallation ToolChain = Installations[0];
        var Resolver = new ModuleDependenciesResolver(TargetRule, SearchedModules, ToolChain);
        Resolver.Resolve();

        var Makefile = await Resolver.GenerateMakefileAsync(TargetRule, SToken);
        await Makefile.ResolveMakefileCacheAsync(ToolChain, TargetRule, SToken);
        var CompileTasks = new CompileTasks(ToolChain);

        int ReturnCode = 0;
        Dictionary<string, List<Task<bool>>> LinkDepends = new();
        if (Makefile.CompileItems.Any())
        {
            int TaskNumber = 0;
            Console.WriteLine("Dispatch {0} tasks with {1} processors.", Makefile.CompileItems.Count, CompileTasks.MaxParallel);
            int Left = 0, Top = 0;
            if (ConsoleExtensions.IsConsoleHandleSupports)
            {
                (Left, Top) = Console.GetCursorPosition();
            }

            foreach (var CompileItem in Makefile.CompileItems)
            {
                Task<bool> CurrentTask = CompileTasks.CompileAsync(CompileItem, TargetRule, SToken).ContinueWith(p =>
                {
                    if (p.IsCompletedSuccessfully)
                    {
                        int Number = Interlocked.Increment(ref TaskNumber);
                        lock (TargetRule)
                        {
                            string Fm = string.Format("[{0}/{1}] {2}", TaskNumber, Makefile.CompileItems.Count, p.Result);
                            if (ConsoleExtensions.IsConsoleHandleSupports)
                            {
                                Console.SetCursorPosition(Left, Top);
                                Fm = Fm.PadRight(Console.BufferWidth, ' ');
                            }
                            Console.WriteLine(Fm);
                        }

                        return true;
                    }
                    else
                    {
                        if (p.Exception!.InnerException is TerminateException TE)
                        {
                            lock (TargetRule)
                            {
                                Console.Error.WriteLine(TE.Message.Trim());
                                if (ConsoleExtensions.IsConsoleHandleSupports)
                                {
                                    (Left, Top) = Console.GetCursorPosition();
                                }
                            }
                            ReturnCode = TE.ErrorCode;
                        }

                        // Rethrowing.
                        return false;
                    }
                });

                if (LinkDepends.TryGetValue(CompileItem.ModuleName, out List<Task<bool>>? LinkTasks) == false)
                {
                    LinkTasks = new();
                    LinkDepends.Add(CompileItem.ModuleName, LinkTasks);
                }
                LinkTasks.Add(CurrentTask);
            }
        }

        Dictionary<string, Task<bool>> Linkers = new();
        foreach (var (ModuleName, _) in Resolver.Modules)
        {
            async Task<bool> StartLinkerAsync(string ModuleName, CancellationToken SToken = default)
            {
                Task<bool>? PreviousTask = null;
                TaskCompletionSource<bool>? TCS = null;
                lock (Linkers)
                {
                    if (Linkers.TryGetValue(ModuleName, out PreviousTask) == false)
                    {
                        TCS = new();
                        PreviousTask = TCS.Task;
                        Linkers.Add(ModuleName, PreviousTask);
                    }
                }

                if (TCS == null)
                {
                    return await PreviousTask;
                }

                try
                {
                    var DependencyCache = Resolver.GetDependencyCache(ModuleName);

                    IEnumerable<Task<bool>> CompileTasks = Enumerable.Empty<Task<bool>>();
                    if (LinkDepends.TryGetValue(ModuleName, out var TasksList))
                    {
                        CompileTasks = TasksList;
                    }

                    bool[] bResults = await Task.WhenAll(CompileTasks.Distinct());
                    if (bResults.Contains(false))
                    {
                        return false;
                    }

                    foreach (var Depend in DependencyCache.DependModules)
                    {
                        if (await StartLinkerAsync(Depend, SToken) == false)
                        {
                            return false;
                        }
                    }

                    var Config = TargetInfo.BuildConfiguration;

                    var Linker = ToolChain.SpawnLinker();
                    string Output = await Linker.LinkAsync(TargetRule, DependencyCache, SToken);
                    Console.Write(Output);

                    TCS.SetResult(true);
                    TCS = null;
                    return true;
                }
                catch (TerminateException TE)
                {
                    Console.Error.WriteLine(TE.Message);
                    ReturnCode = TE.ErrorCode;
                    return false;
                }
                catch (Exception E)
                {
                    Console.Error.WriteLine(E);
                    return false;
                }
                finally
                {
                    TCS?.SetResult(false);
                }
            }

            _ = StartLinkerAsync(ModuleName, SToken);
        }

        await Task.WhenAll(Linkers.Values);
        await Makefile.SaveMakefileCacheAsync(TargetRule, SToken);
        return ReturnCode;
    }
}
