// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

using AE.Rules;
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

    private readonly Dictionary<string, ModuleInformation> DependencyCaches = new();

    public void Resolve()
    {
        Debug.Assert(DependencyCaches.Count == 0);
        SearchDependencyRecursive(Rule.TargetModuleName, true);
    }

    private void SearchDependencyRecursive(string Current, bool bPrimary)
    {
        if (DependencyCaches.TryGetValue(Current, out ModuleInformation? DependencyCache) == false)
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
                ModuleInformation? DependCache = DependencyCaches[Depend];
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

    public ModuleInformation GetDependencyCache(string Module)
    {
        return DependencyCaches[Module];
    }

    public IEnumerable<ModuleRules> GetModules() => Modules.Values.Select(p => p.Rule);

    public async Task<Dictionary<ModuleInformation, Makefile>> GenerateMakefilesAsync(TargetRules Rule, CancellationToken SToken = default)
    {
        var Config = Rule.Target.BuildConfiguration;
        List<Task<(ModuleInformation, Makefile)>> Tasks = new();

        foreach (var (Name, Cache) in DependencyCaches)
        {
            string MakefileDir = Path.Combine(Cache.ProjectDir.Intermediate.Build, Config.Platform.TargetName, Config.Configuration.ToString(), Name);
            Tasks.Add(Makefile.LoadMakefileCacheAsync(MakefileDir).ContinueWith(p =>
            {
                Makefile Makefile = p.Result;
                var CacheDict = Makefile.Caches.ToDictionary(p => p.SourceCache.Path, p => p);
                List<MakefileCompile> CompileItems = new();

                foreach (var SourceFile in Cache.SourceFiles)
                {
                    if (Global.IsSourceCode(SourceFile) == false)
                    {
                        continue;
                    }

                    MakefileCompile? Compile = null;

                    if (CacheDict.TryGetValue(SourceFile, out MakefileSourceCache? SourceCache))
                    {
                        if (SourceCache.IsNewer(SourceFile) == false)
                        {
                            Compile = new()
                            {
                                Path = SourceFile,
                                Cache = SourceCache,
                                ModuleInfo = Cache
                            };
                        }
                    }

                    // source is newer.
                    if (Compile == null)
                    {
                        Compile = new()
                        {
                            Path = SourceFile,
                            Cache = null,
                            ModuleInfo = Cache
                        };
                    }

                    CompileItems.Add(Compile);
                }

                Makefile.CompileItems = CompileItems.ToArray();
                return (Cache, Makefile);
            }));
        }

        var Items = await Task.WhenAll(Tasks);
        return Items.ToDictionary(p => p.Item1, p => p.Item2);
    }
}
