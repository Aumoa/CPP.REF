﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.CLI;
using AE.Exceptions;
using AE.Misc;
using AE.Platform;
using AE.Platform.Windows;
using AE.Projects;
using AE.Rules;
using AE.Source;
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
        Dictionary<string, (ModuleRules, ProjectDirectory, string)> SearchedModules = new();
        SearchCXXModulesRecursive(Workspace, TargetRule, SearchedModules, TargetRule.Name, TargetRule.TargetModuleName);

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
        var Tasks = new CompileTasks(ToolChain);

        int ReturnCode = 0;
        if (Makefile.CompileItems.Any())
        {
            var DeferredTasks = new List<Task>();
            int TaskNumber = 0;
            Console.WriteLine("Dispatch {0} tasks with {1} processors.", Makefile.CompileItems.Count, Tasks.MaxParallel);
            int Left = 0, Top = 0;
            if (ConsoleExtensions.IsConsoleHandleSupports)
            {
                (Left, Top) = Console.GetCursorPosition();
            }
            foreach (var CompileItem in Makefile.CompileItems)
            {
                DeferredTasks.Add(Tasks.CompileAsync(CompileItem, TargetRule, SToken).ContinueWith(p =>
                {
                    if (p.IsCompletedSuccessfully)
                    {
                        int Number = Interlocked.Increment(ref TaskNumber);
                        lock (DeferredTasks)
                        {
                            string Fm = string.Format("[{0}/{1}] {2}", TaskNumber, Makefile.CompileItems.Count, p.Result);
                            if (ConsoleExtensions.IsConsoleHandleSupports)
                            {
                                Console.SetCursorPosition(Left, Top);
                                Fm = Fm.PadRight(Console.BufferWidth, ' ');
                            }
                            Console.WriteLine(Fm);
                        }
                    }
                    else
                    {
                        if (p.Exception!.InnerException is TerminateException TE)
                        {
                            lock (DeferredTasks)
                            {
                                Console.Error.WriteLine(TE.Message.Trim());
                                if (ConsoleExtensions.IsConsoleHandleSupports)
                                {
                                    (Left, Top) = Console.GetCursorPosition();
                                }
                            }
                            ReturnCode = TE.ErrorCode;
                        }
                        else
                        {
                            var _ = p.Result;
                        }
                    }
                }));
            }

            await Task.WhenAll(DeferredTasks);
            await Makefile.SaveMakefileCacheAsync(TargetRule, SToken);
        }
        else
        {
            Console.WriteLine("No compile items.");
        }

        return ReturnCode;
    }

    private static void SearchCXXModulesRecursive(Workspace InWorkspace, TargetRules Rule, Dictionary<string, (ModuleRules, ProjectDirectory, string)> SearchedModules, string FromModule, string CurrentModule)
    {
        if (SearchedModules.TryGetValue(CurrentModule, out var ModuleRule) == false)
        {
            ACXXModule? Module = InWorkspace.SearchCXXModuleByName(CurrentModule);
            if (Module == null)
            {
                throw new TerminateException(8, CoreStrings.Errors.DependencyModuleNotFound, FromModule, CurrentModule);
            }

            ModuleRule.Item1 = Module.GenerateModuleRule(Rule);
            ModuleRule.Item2 = Module.ProjectDirectory;
            ModuleRule.Item3 = Module.SourcePath;
            SearchedModules.Add(CurrentModule, ModuleRule);

            foreach (var NextModule in ModuleRule.Item1.PublicDependencyModuleNames.Concat(ModuleRule.Item1.PrivateDependencyModuleNames))
            {
                SearchCXXModulesRecursive(InWorkspace, Rule, SearchedModules, CurrentModule, NextModule);
            }
        }
    }
}
