// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

using AE.BuildSettings;
using AE.Exceptions;
using AE.Rules;
using AE.Source;
using AE.SourceTree;
using AE.System;

using Microsoft.CodeAnalysis;

namespace AE.Projects;

public class ModuleDependenciesResolver
{
    public required TargetRules Rule { get; init; }

    public required Dictionary<string, SearchedModule> Modules { get; init; } = new();

    public required ToolChainInstallation ToolChain { get; init; }

    [SetsRequiredMembers]
    public ModuleDependenciesResolver(TargetRules Rule, Dictionary<string, SearchedModule> Modules, ToolChainInstallation ToolChain)
    {
        this.Rule = Rule;
        this.Modules = Modules;
        this.ToolChain = ToolChain;
    }

    public record ModuleDependencyCache
    {
        public required string Name { get; init; }

        public required ProjectDirectory ProjectDir { get; init; }

        public required TargetType TargetType { get; init; }

        public required string[] IncludePaths { get; init; }

        public required string[] AdditionalMacros { get; init; }

        public required string[] SourceFiles { get; init; }

        public required string[] DependModules { get; init; }

        public required string[] AdditionalLibraries { get; init; }

        public required int[] DisableWarnings { get; init; }
    }

    private readonly Dictionary<string, ModuleDependencyCache> DependencyCaches = new();

    public void Resolve()
    {
        Debug.Assert(DependencyCaches.Count == 0);
        SearchDependencyRecursive(Rule.TargetModuleName, true);
    }

    private void SearchDependencyRecursive(string Current, bool bPrimary)
    {
        if (DependencyCaches.TryGetValue(Current, out ModuleDependencyCache? DependencyCache) == false)
        {
            var CurrentSearchedModule = Modules[Current];
            string SourcePath = CurrentSearchedModule.Location;
            var ModuleRule = CurrentSearchedModule.Rule;
            var ProjectDir = CurrentSearchedModule.ProjectDir;

            string AsFullPath(string CurrentPath)
            {
                return Path.Combine(SourcePath, CurrentPath);
            }

            List<string> IncludePaths = new();
            List<string> AdditionalMacros = new();
            List<string> ApiDescriptions = new();
            List<string> AdditionalLibraries = new();
            List<int> DisableWarnings = new();

            foreach (var Depend in ModuleRule.PublicDependencyModuleNames.Concat(ModuleRule.PrivateDependencyModuleNames))
            {
                SearchDependencyRecursive(Depend, false);
                ModuleDependencyCache? DependCache = DependencyCaches[Depend];
                IncludePaths.AddRange(DependCache.IncludePaths);
                AdditionalMacros.AddRange(DependCache.AdditionalMacros);
                ApiDescriptions.AddRange(DependCache.DependModules);
                ApiDescriptions.Add(Depend);
                AdditionalLibraries.AddRange(DependCache.AdditionalLibraries);
                DisableWarnings.AddRange(DependCache.DisableWarnings);
            }

            IncludePaths.AddRange(ModuleRule.PublicIncludePaths.Select(AsFullPath));
            IncludePaths.AddRange(ModuleRule.PrivateIncludePaths.Select(AsFullPath));
            AdditionalMacros.AddRange(ModuleRule.PublicAdditionalMacros);
            AdditionalMacros.AddRange(ModuleRule.PrivateAdditionalMacros);
            AdditionalLibraries.AddRange(ModuleRule.PublicAdditionalLibraries);
            AdditionalLibraries.AddRange(ModuleRule.PrivateAdditionalLibraries);
            DisableWarnings.AddRange(ModuleRule.PublicDisableWarnings);
            DisableWarnings.AddRange(ModuleRule.PrivateDisableWarnings);

            DependencyCaches[Current] = new()
            {
                Name = Current,
                TargetType = bPrimary ? Rule.Type : TargetType.Module,
                ProjectDir = ProjectDir,
                IncludePaths = IncludePaths.Distinct().ToArray(),
                AdditionalMacros = AdditionalMacros.Distinct().ToArray(),
                SourceFiles = Directory.GetFiles(SourcePath, "*", SearchOption.AllDirectories).Where(Global.IsSourceFile).ToArray(),
                DependModules = ApiDescriptions.Distinct().ToArray(),
                AdditionalLibraries = AdditionalLibraries.Distinct().ToArray(),
                DisableWarnings = DisableWarnings.Distinct().ToArray(),
            };
        }
    }

    public ModuleDependencyCache GetDependencyCache(string Module)
    {
        return DependencyCaches[Module];
    }

    public IEnumerable<ModuleRules> GetModules() => Modules.Values.Select(p => p.Rule);

    public async Task<Makefile> GenerateMakefileAsync(TargetRules Rule, CancellationToken SToken = default)
    {
        var Config = Rule.Target.BuildConfiguration;
        var Makefile = new Makefile() { Items = new() };
        List<Task> Tasks = new();

        foreach (var (Name, Cache) in DependencyCaches)
        {
            foreach (var SourceFile in Cache.SourceFiles)
            {
                if (Global.IsSourceCode(SourceFile))
                {
                    Tasks.Add(SourceCodeHash.GenerateHashAsync(SourceFile).ContinueWith(p =>
                    {
                        string Intermediate = Cache.ProjectDir.Intermediate.Root;
                        string Output = Path.Combine(Cache.ProjectDir.Intermediate.Build, Config.Platform.TargetName, Config.Configuration.ToString(), Name);

                        var Item = new MakefileCompile
                        {
                            SourceCode = SourceFile,
                            Hash = p.Result,
                            ModuleName = Name,
                            Intermediate = Intermediate,
                            DependModules = Cache.DependModules,
                            IncludePaths = Cache.IncludePaths,
                            AdditionalMacros = Cache.AdditionalMacros,
                            DisableWarnings = Cache.DisableWarnings,
                            Output = Output,
                            IncludeHashes = new()
                        };

                        lock (Makefile)
                        {
                            Makefile.Items.Add(Item);
                        }
                    }));
                }
            }
        }

        await Task.WhenAll(Tasks);
        return Makefile;
    }
}
