// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.Contracts;

using AE.CompilerServices;
using AE.Exceptions;
using AE.Platform;
using AE.Rules;
using AE.Source;

namespace AE.Projects;

public class Workspace
{
    private readonly Dictionary<string, ATarget> Targets = new();
    private readonly Dictionary<string, CppModule> CXXModules = new();
    private readonly List<CSharpModule> CSModules = new();

    public Workspace()
    {
    }

    public ProjectDirectory CurrentTarget { get; private set; }

    public async Task ConfigureWorkspaceAsync(ProjectDirectory Target, bool bEngineCode, CancellationToken SToken = default)
    {
        await CollectModuleInSubdirectoryRecursiveAsync(Target, Target.Source.Root, SToken);
        CurrentTarget = Target;
    }

    private async Task CollectModuleInSubdirectoryRecursiveAsync(ProjectDirectory Target, string InSubdirectory, CancellationToken SToken = default)
    {
        foreach (var ModulePath in Directory.GetFiles(InSubdirectory, "*", SearchOption.TopDirectoryOnly))
        {
            if (InSubdirectory.EndsWith("Scripts"))
            {
                return;
            }

            if (ModulePath.EndsWith("csproj", StringComparison.OrdinalIgnoreCase))
            {
                CSModules.Add(new CSharpModule(Target, InSubdirectory));
                return;
            }

            if (ModulePath.EndsWith("Module.cs", StringComparison.OrdinalIgnoreCase))
            {
                var CXXModule = new CppModule(Target, Path.GetRelativePath(Target.Source.Root, InSubdirectory));
                await CXXModule.ConfigureAsync(SToken);
                CXXModules.Add(CXXModule.Name, CXXModule);
            }

            if (ModulePath.EndsWith("Target.cs", StringComparison.OrdinalIgnoreCase))
            {
                var RelativePath = Path.GetRelativePath(Target.Source.Root, ModulePath);
                var Instance = new ATarget(Target, RelativePath);
                Targets.Add(Instance.TargetName, Instance);
            }
        }

        foreach (var Subdirectory in Directory.GetDirectories(InSubdirectory, "*", SearchOption.TopDirectoryOnly))
        {
            await CollectModuleInSubdirectoryRecursiveAsync(Target, Subdirectory, SToken);
        }
    }

    public IEnumerable<CSharpModule> GetCSModules() => CSModules;

    public IEnumerable<CppModule> GetCXXModules() => CXXModules.Values;

    public ATarget? SearchTargetByName(string InTargetName)
    {
        Targets.TryGetValue(InTargetName, out ATarget? SearchInstance);
        return SearchInstance;
    }

    public CppModule? SearchCXXModuleByName(string InModuleName)
    {
        CXXModules.TryGetValue(InModuleName, out CppModule? SearchInstance);
        return SearchInstance;
    }

    public void SearchModules()
    {
        SearchedModuleCache.Clear();

        foreach (var (name, module) in CXXModules)
        {
            var searchedModule = new SearchedModule
            {
                Rules = module.GenerateModuleRule(Target.Rules),
                ProjectDirectory = Target.ProjectDirectory,
                SourcePath = module.SourcePath
            };

            SearchedModuleCache.TryAdd(name, searchedModule);
        }
    }

    public void SearchCXXModulesRecursive(TargetRules Rule, string FromModule, string CurrentModule)
    {
        if (SearchedModuleCache.TryGetModule(CurrentModule, out var ModuleRule) == false)
        {
            CppModule? Module = SearchCXXModuleByName(CurrentModule);
            if (Module == null)
            {
                throw new TerminateException(KnownErrorCode.ModuleNotFound, CoreStrings.Errors.DependencyModuleNotFound(FromModule, CurrentModule));
            }

            ModuleRule = new()
            {
                Rules = Module.GenerateModuleRule(Rule),
                ProjectDirectory = Module.ProjectDirectory,
                SourcePath = Module.SourcePath
            };
            SearchedModuleCache.TryAdd(CurrentModule, ModuleRule);

            foreach (var NextModule in ModuleRule.Rules.PublicDependencyModuleNames.Concat(ModuleRule.Rules.PrivateDependencyModuleNames))
            {
                SearchCXXModulesRecursive(Rule, CurrentModule, NextModule);
            }
        }
    }
}
