// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Exceptions;
using AE.Rules;
using AE.Source;

namespace AE.Projects;

public class Workspace
{
    private readonly Dictionary<string, ATarget> Targets = new();
    private readonly Dictionary<string, ACXXModule> CXXModules = new();
    private readonly List<ACSModule> CSModules = new();
    private readonly List<AScriptModule> ScriptModules = new();

    public Workspace()
    {
    }

    public ProjectDirectory CurrentTarget { get; private set; }

    public async Task ConfigureWorkspaceAsync(ProjectDirectory Target, bool bEngineCode, CancellationToken SToken = default)
    {
        await CollectModuleInSubdirectoryRecursiveAsync(Target, Target.Source.Root, SToken);
        CurrentTarget = Target;
    }

    private static bool IsSourceCode(string Filename)
    {
        string Ext = Path.GetExtension(Filename).ToLower();
        return Ext == ".cpp" || Ext == ".h" || Ext == ".ixx" || Ext == ".cs";
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
                CSModules.Add(new ACSModule(Target, InSubdirectory));
                return;
            }

            if (ModulePath.EndsWith("Module.cs", StringComparison.OrdinalIgnoreCase))
            {
                var CXXModule = new ACXXModule(Target, Path.GetRelativePath(Target.Source.Root, InSubdirectory));
                await CXXModule.ConfigureAsync(SToken);
                CXXModules.Add(CXXModule.ModuleName, CXXModule);

                var ScriptPath = Path.Combine(InSubdirectory, "Scripts");
                if (Directory.Exists(ScriptPath))
                {
                    ScriptModules.Add(new AScriptModule(Target, InSubdirectory, CXXModule));
                }
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

    public IEnumerable<ACSModule> GetCSModules() => CSModules;

    public IEnumerable<AScriptModule> GetScriptModules() => ScriptModules;

    public IEnumerable<ACXXModule> GetCXXModules() => CXXModules.Values;

    public ATarget? SearchTargetByName(string InTargetName)
    {
        Targets.TryGetValue(InTargetName, out ATarget? SearchInstance);
        return SearchInstance;
    }

    public ACXXModule? SearchCXXModuleByName(string InModuleName)
    {
        CXXModules.TryGetValue(InModuleName, out ACXXModule? SearchInstance);
        return SearchInstance;
    }

    public void SearchCXXModulesRecursive(TargetRules Rule, Dictionary<string, SearchedModule> SearchedModules, string FromModule, string CurrentModule)
    {
        if (SearchedModules.TryGetValue(CurrentModule, out var ModuleRule) == false)
        {
            ACXXModule? Module = SearchCXXModuleByName(CurrentModule);
            if (Module == null)
            {
                throw new TerminateException(KnownErrorCode.ModuleNotFound, CoreStrings.Errors.DependencyModuleNotFound(FromModule, CurrentModule));
            }

            ModuleRule = new()
            {
                Rule = Module.GenerateModuleRule(Rule),
                ProjectDir = Module.ProjectDirectory,
                Location = Module.SourcePath
            };
            SearchedModules.Add(CurrentModule, ModuleRule);

            foreach (var NextModule in ModuleRule.Rule.PublicDependencyModuleNames.Concat(ModuleRule.Rule.PrivateDependencyModuleNames))
            {
                SearchCXXModulesRecursive(Rule, SearchedModules, CurrentModule, NextModule);
            }
        }
    }
}
