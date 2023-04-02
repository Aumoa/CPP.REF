// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Compilation;
using AE.Rules;
using AE.Source;

using System.Collections.Concurrent;
using System.Data;
using System.Diagnostics;
namespace AE.Projects;

[DebuggerDisplay("Name = {Name}")]
public class CXXProject
{
    private readonly List<string> Modules = new();
    private readonly ConcurrentDictionary<string, ResolvedModule> CompiledRules = new();

    public string TargetFile { get; }

    public string Name => Path.GetFileNameWithoutExtension(TargetFile).Replace(".Target", string.Empty);

    public TargetRules Rules { get; }

    public string SourceCodeDirectory => Path.GetDirectoryName(TargetFile)!;

    public SourceCodeDirectory Workspace { get; }

    public record ResolvedModule
    {
        public string Path { get; set; } = null!;

        public ModuleRules Rules { get; set; } = null!;

        public string[] DependencyModules { get; set; } = null!;

        public string[] SourceFiles { get; set; } = null!;

        public string[] IncludePaths { get; set; } = null!;

        public string[] AdditionalMacros { get; set; } = null!;

        public int[] DisableWarnings { get; set; } = null!;

        public string[] AdditionalLibraries { get; set; } = null!;
    }

    public CXXProject(string InTargetFile, TargetRules InRules, SourceCodeDirectory InWorkspace)
    {
        TargetFile = InTargetFile;
        Rules = InRules;
        Workspace = InWorkspace;

        string SourceDirectory = Path.GetDirectoryName(InTargetFile)!;
        SearchModulesRecursive(SourceDirectory, true);
    }

    public async Task<ModuleRules> CompileModuleAsync(IDictionary<string, ResolvedModule> Projects, string TargetModule, CancellationToken CToken = default)
    {
        if (Modules.Contains(TargetModule) == false)
        {
            throw new InvalidOperationException(CoreStrings.Errors.DependencyModuleNotFound);
        }

        Type ModuleType = await CSCompiler.LoadClassAsync<ModuleRules>(TargetModule, CToken);

        var Ctor = ModuleType.GetConstructor(new[] { typeof(TargetRules) })
            ?? throw new InvalidOperationException(string.Format(CoreStrings.Errors.ModuleRuleConstructorNotFound, ModuleType.Name));
        var Rule = (ModuleRules)Ctor.Invoke(new object[] { Rules });

        HashSet<string> SourceFiles = new();
        string ModuleDirectory = Path.GetDirectoryName(TargetModule)!;
        foreach (var SourceFile in Directory.GetFiles(ModuleDirectory, "*.*", SearchOption.AllDirectories))
        {
            SourceFiles.Add(SourceFile);
        }

        var Resolved = new ResolvedModule
        {
            Path = ModuleDirectory,
            Rules = Rule,
            SourceFiles = SourceFiles.ToArray()
        };

        bool bStatus = CompiledRules.TryAdd(TargetModule, Resolved);
        Debug.Assert(bStatus);
        Projects.Add(Resolved.Rules.Name, Resolved);

        return Rule;
    }

    public async Task CompileModulesAsync(IDictionary<string, ResolvedModule> Projects, CancellationToken CToken = default)
    {
        List<Task> Tasks = new();
        foreach (var ModuleName in Modules)
        {
            Tasks.Add(CompileModuleAsync(Projects, ModuleName, CToken));
        }

        await Task.WhenAll(Tasks);
    }

    public void ResolveDependency(IDictionary<string, ResolvedModule> Projects)
    {
        HashSet<ResolvedModule> IncludeSet = new();
        List<ResolvedModule> PriorityList = new();
        Dictionary<ResolvedModule, ResolvedModule[]> DependenciesMap = new();

        foreach (var (_, Project) in Projects)
        {
            ComputeDependencyRecursive(Projects, Project, IncludeSet, PriorityList, DependenciesMap);
        }

        foreach (var (_, Project) in Projects)
        {
            BuildDependencies(Project, DependenciesMap[Project]);
        }
    }

    private void ComputeDependencyRecursive(IDictionary<string, ResolvedModule> Projects, ResolvedModule Rule, HashSet<ResolvedModule> IncludeSet, List<ResolvedModule> PriorityList, Dictionary<ResolvedModule, ResolvedModule[]> DependenciesMap)
    {
        if (IncludeSet.Contains(Rule))
        {
            return;
        }

        List<string> Dependencies = new();
        Dependencies.AddRange(Rule.Rules.PublicDependencyModuleNames);
        Dependencies.AddRange(Rule.Rules.PrivateDependencyModuleNames);

        List<ResolvedModule> DependProjects = new();

        foreach (var Depend in Dependencies)
        {
            if (Projects.TryGetValue(Depend, out var DependProject) == false)
            {
                throw new KeyNotFoundException(string.Format(CoreStrings.Errors.DependencyModuleNotFound, Rule.Rules.Name, Depend));
            }
            if (!IncludeSet.Contains(DependProject))
            {
                ComputeDependencyRecursive(Projects, DependProject, IncludeSet, PriorityList, DependenciesMap);
            }

            DependProjects.AddRange(DependenciesMap[DependProject]);
        }

        DependProjects.AddRange(Dependencies.Select(p =>
        {
            return Projects[p];
        }));

        DependenciesMap.Add(Rule, DependProjects.Distinct().ToArray());
        IncludeSet.Add(Rule);
        PriorityList.Add(Rule);
    }

    private void BuildDependencies(ResolvedModule Project, ResolvedModule[] Dependencies)
    {
        List<string> IncludePaths = new();
        List<string> AdditionalMacros = new();
        List<int> DisableWarnings = new();
        List<string> AdditionalLibraries = new();

        IEnumerable<string> ProcessIncludes(ResolvedModule Project, IEnumerable<string> Paths)
        {
            return Paths.Select(p => Path.Combine(Project.Path, p));
        }

        List<string> APIDefinitions = new()
        {
            $"{Project.Rules.GenerateSafeAPIName()}=${{CMAKE_SHARED_LIBRARY_EXPORT}}"
        };

        foreach (var Depend in Dependencies)
        {
            IncludePaths.AddRange(ProcessIncludes(Depend, Depend.Rules.PublicIncludePaths));
            AdditionalMacros.AddRange(Depend.Rules.PublicAdditionalMacros);
            DisableWarnings.AddRange(Depend.Rules.PublicDisableWarnings);
            AdditionalLibraries.AddRange(Depend.Rules.PublicAdditionalLibraries);

            APIDefinitions.Add($"{Depend.Rules.GenerateSafeAPIName()}=${{CMAKE_SHARED_LIBRARY_IMPORT}}");
        }

        IncludePaths.AddRange(ProcessIncludes(Project, Project.Rules.PublicIncludePaths));
        IncludePaths.AddRange(ProcessIncludes(Project, Project.Rules.PrivateIncludePaths));

        // Add generated.h include path.
        IncludePaths.Add(Path.Combine(Workspace.GeneratedDirectory, Project.Rules.Name));

        Project.IncludePaths = IncludePaths.Distinct().ToArray();

        AdditionalMacros.AddRange(Project.Rules.PublicAdditionalMacros);
        AdditionalMacros.AddRange(Project.Rules.PrivateAdditionalMacros);
        AdditionalMacros.AddRange(APIDefinitions);
        Project.AdditionalMacros = AdditionalMacros.Distinct().ToArray();

        DisableWarnings.AddRange(Project.Rules.PublicDisableWarnings);
        DisableWarnings.AddRange(Project.Rules.PrivateDisableWarnings);
        Project.DisableWarnings = DisableWarnings.Distinct().ToArray();

        AdditionalLibraries.AddRange(Project.Rules.PublicAdditionalLibraries);
        AdditionalLibraries.AddRange(Project.Rules.PrivateAdditionalLibraries);
        Project.AdditionalLibraries = AdditionalLibraries.Distinct().ToArray();

        Project.DependencyModules = Dependencies.Select(p => p.Rules.Name).ToArray();
    }

    public string[] GetAllModuleSourceFiles()
    {
        HashSet<string> SourceFiles = new();

        foreach (var Module in Modules)
        {
            string ModuleDirectory = Path.GetDirectoryName(Module)!;
            foreach (var SourceFile in Directory.GetFiles(ModuleDirectory, "*.*", SearchOption.AllDirectories))
            {
                SourceFiles.Add(SourceFile);
            }
        }

        SourceFiles.Add(TargetFile);
        return SourceFiles.ToArray();
    }

    public string[] GetModules()
    {
        return Modules.ToArray();
    }

    public string[] GetProjectDefinitions()
    {
        static char Int(bool Value) => Value ? '1' : '0';
        List<string> Definitions = new();
        PlatformGroup.ForEach(p =>
        {
            bool b = Rules.Platform.Group == p;
            Definitions.Add($"{p.ToDefinition()}={Int(b)}");
        });
        Definitions.AddRange(new[]
        {
            "PLATFORM_WINDOWS=1",
            "UNICODE=1",
            "SHIPPING=1",
            "DO_CHECK=0"
        });
        return Definitions.ToArray();
    }

    public ResolvedModule? GetModuleRule(string ModuleName)
    {
        if (CompiledRules.TryGetValue(ModuleName, out ResolvedModule? Value))
        {
            return Value;
        }
        else
        {
            return null;
        }
    }

    private void SearchModulesRecursive(string SourceDirectory, bool bFirst)
    {
        string? ModuleFile = null;

        foreach (var Path in Directory.GetFiles(SourceDirectory, "*.cs", SearchOption.TopDirectoryOnly))
        {
            if (Path.Contains(".Target.cs") && bFirst == false)
            {
                return;
            }

            if (Path.Contains(".Module.cs"))
            {
                if (ModuleFile != null)
                {
                    throw new InvalidOperationException(CoreStrings.Errors.DuplicatedModuleDefinitions);
                }

                ModuleFile = Path;
            }
        }

        if (ModuleFile != null)
        {
            Modules.Add(ModuleFile);
            return;
        }

        foreach (var Subdirectory in Directory.GetDirectories(SourceDirectory, "*", SearchOption.TopDirectoryOnly))
        {
            SearchModulesRecursive(Subdirectory, false);
        }
    }
}
