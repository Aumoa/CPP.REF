// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.CLI;
using AE.Exceptions;
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
        public bool? bClean { get; set; }
    }

    private readonly Arguments BuildArgs = new();

    public BuildExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(BuildArgs);
    }

    public async Task<int> RunAsync(CancellationToken SToken = default)
    {
        Workspace Workspace = await ConfigureWorkspaceAsync(SToken);
        if (BuildArgs.bClean == true)
        {
            foreach (var Dir in GetProjectDirectories())
            {
                Dir.Cleanup();
            }
        }

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

        var Installations = VisualStudioInstallation.FindVisualStudioInstallations(Platform.Windows.Compiler.VisualStudio2022);
        if (Installations.Any() == false)
        {
            throw new InvalidOperationException("Internal error.");
        }

        ToolChainInstallation ToolChain = Installations[0];
        var Resolver = new ModuleDependenciesResolver(TargetRule, SearchedModules, ToolChain);
        Resolver.Resolve();

        var Tree = new CompileTree(Resolver, ToolChain);
        await Tree.ScanDependenciesAsync(SToken);
        await Tree.CompileAsync(SToken);

        return 0;
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
