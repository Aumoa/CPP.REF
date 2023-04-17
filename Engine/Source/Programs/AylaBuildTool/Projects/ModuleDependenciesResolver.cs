// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Exceptions;
using AE.Rules;
using AE.Source;

using Microsoft.CodeAnalysis;

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

namespace AE.Projects;

public class ModuleDependenciesResolver
{
    private ProjectDirectory[] Projects { get; }

    [SetsRequiredMembers]
    public ModuleDependenciesResolver(params ProjectDirectory[] Projects)
    {
        this.Projects = Projects;
    }

    private readonly Dictionary<ProjectDirectory, List<ACXXModule>> DirectoryModuleMap = new();
    private readonly Dictionary<string, ACXXModule> ModuleMap = new();
    private readonly Dictionary<string, ProjectDirectory> DirectoryMap = new();

    public async Task<TargetRules> ResolveAsync(string TargetName, TargetInfo TargetInfo, CancellationToken SToken = default)
    {
        string TargetFilename = TargetName + ".Target.cs";

        ProjectDirectory TargetDirectory = new();
        string? TargetPath = null;

        foreach (var Workspace in Projects)
        {
            foreach (var File in Directory.GetFiles(Workspace.Source.Root, "*.Target.cs"))
            {
                if (Path.GetFileName(File).Equals(TargetName + ".Target.cs", StringComparison.OrdinalIgnoreCase))
                {
                    if (TargetPath != null)
                    {
                        throw new TerminateException(5, CoreStrings.Errors.TargetRuleDuplicated);
                    }

                    TargetPath = File;
                    TargetDirectory = Workspace;
                }
            }
        }

        if (TargetPath == null)
        {
            throw new TerminateException(6, CoreStrings.Errors.TargetNotFoundException, TargetFilename);
        }

        var Target = new ATarget(TargetDirectory, TargetName);
        await Target.ConfigureAsync(SToken);
        TargetRules Rule = Target.GenerateTargetRule(TargetInfo);

        Dictionary<string, ACXXModule> Modules = await LoadModulesAsync(SToken);
        if (Modules.TryGetValue(Rule.TargetModuleName, out ACXXModule? PrimaryModule) == false)
        {
            throw new TerminateException(7, CoreStrings.Errors.DependencyModuleNotFound, Target.TargetName, Rule.TargetModuleName);
        }

        ResolveDependenciesAsync(Modules, PrimaryModule, Rule);
        return Rule;
    }

    public Dictionary<string, ACXXModule> RequiredModules = new();

    private void ResolveDependenciesAsync(Dictionary<string, ACXXModule> Sources, ACXXModule CurrentModule, TargetRules Rule)
    {
        Debug.Assert(RequiredModules != null);

        if (RequiredModules.TryAdd(CurrentModule.ModuleName, CurrentModule) == false)
        {
            return;
        }

        var ModuleRule = CurrentModule.GenerateModuleRule(Rule);
        void Resolve(string? Depend)
        {
            if (string.IsNullOrEmpty(Depend) || Sources.TryGetValue(Depend, out ACXXModule? Module) == false)
            {
                throw new TerminateException(7, CoreStrings.Errors.DependencyModuleNotFound, Rule.Name, Depend);
            }

            ResolveDependenciesAsync(Sources, Module, Rule);
        }

        foreach (var Depend in ModuleRule.PublicDependencyModuleNames)
        {
            Resolve(Depend);
        }
        foreach (var Depend in ModuleRule.PrivateDependencyModuleNames)
        {
            Resolve(Depend);
        }
    }

    public ProjectDirectory GetModuleDirectory(ACXXModule Module)
    {
        return DirectoryMap[Module.ModuleName];
    }

    private async Task<Dictionary<string, ACXXModule>> LoadModulesAsync(CancellationToken SToken = default)
    {
        Dictionary<string, ACXXModule> Modules = new();
        void AddModules(ProjectDirectory Dir)
        {
            foreach (var SourceCode in Directory.GetFiles(Dir.Source.Root, "*.Module.cs", SearchOption.AllDirectories))
            {
                string? DirectoryName = Path.GetDirectoryName(SourceCode);
                if (DirectoryName == null)
                {
                    continue;
                }

                var Module = new ACXXModule(Dir, DirectoryName);
                Modules.Add(Module.ModuleName, Module);

                if (this.DirectoryModuleMap.TryGetValue(Dir, out var List) == false)
                {
                    List = new();
                    this.DirectoryModuleMap.Add(Dir, List);
                }

                List.Add(Module);
                ModuleMap.Add(Module.ModuleName, Module);
                DirectoryMap.Add(Module.ModuleName, Dir);
            }
        }

        foreach (var Workspace in Projects)
        {
            AddModules(Workspace);
        }

        List<Task> Tasks = new();
        foreach (var Module in Modules.Values)
        {
            Tasks.Add(Module.ConfigureAsync(SToken));
        }

        await Task.WhenAll(Tasks);
        return Modules;
    }

    public string[] ResolveIncludeDirectories(ModuleRules Rule, bool bCurrentModule)
    {
        Debug.Assert(RequiredModules != null);
        List<string> Includes = new();

        foreach (var Depend in Rule.PublicDependencyModuleNames)
        {
            if (RequiredModules.TryGetValue(Depend, out var Required) == false)
            {
                throw new TerminateException(7, CoreStrings.Errors.DependencyModuleNotFound, Rule.Name, Depend);
            }

            Includes.AddRange(ResolveIncludeDirectories(Required.GenerateModuleRule(Rule.TargetRule), false));
        }

        string SourceDirectory = ModuleMap[Rule.Name].SourcePath;
        foreach (var Include in Rule.PublicIncludePaths)
        {
            string IncludePath = Path.Combine(SourceDirectory, Include);
            Includes.Add(IncludePath);
        }

        if (bCurrentModule)
        {
            foreach (var Include in Rule.PrivateIncludePaths)
            {
                string IncludePath = Path.Combine(SourceDirectory, Include);
                Includes.Add(IncludePath);
            }
        }

        return Includes.Distinct().ToArray();
    }

    public string[] ResolveDefines(ModuleRules Rule, bool bCurrentModule)
    {
        Debug.Assert(RequiredModules != null);
        List<string> Macros = new();

        foreach (var Depend in Rule.PublicDependencyModuleNames)
        {
            if (RequiredModules.TryGetValue(Depend, out var Required) == false)
            {
                throw new TerminateException(7, CoreStrings.Errors.DependencyModuleNotFound, Rule.Name, Depend);
            }

            Macros.AddRange(ResolveDefines(Required.GenerateModuleRule(Rule.TargetRule), false));
        }

        foreach (var Macro in Rule.PublicAdditionalMacros)
        {
            Macros.Add(Macro);
        }

        if (bCurrentModule)
        {
            foreach (var Macro in Rule.PrivateAdditionalMacros)
            {
                Macros.Add(Macro);
            }

            Macros.Add($"{Rule.GenerateSafeAPIName()}=__declspec(dllexport)");
        }
        else
        {
            Macros.Add($"{Rule.GenerateSafeAPIName()}=__declspec(dllimport)");
        }

        Dictionary<string, bool> PlatformDefines = new()
        {
            ["PLATFORM_WINDOWS"] = false,
            ["PLATFORM_LINUX"] = false
        };

        if (Rule.TargetRule.Platform.Group == PlatformGroup.Windows)
        {
            PlatformDefines["PLATFORM_WINDOWS"] = true;
        }

        foreach (var (Macro, bEnabled) in PlatformDefines)
        {
            Macros.Add($"{Macro}={(bEnabled ? 1 : 0)}");
        }

        return Macros.Distinct().ToArray();
    }
}
