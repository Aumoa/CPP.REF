// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Data;

using AE.Extensions;
using AE.ProjectFiles;
using AE.Rules;

namespace AE.Projects;

public static class ModuleDependencyCache
{
    private static readonly Dictionary<string, ModuleInformation> s_Cached = new();

    public static IEnumerable<ModuleInformation> Modules => s_Cached.Values;

    public static void Clear()
    {
        lock (s_Cached)
        {
            s_Cached.Clear();
        }
    }

    public static void BuildCache(string name) => GetCached(name);

    public static ModuleInformation GetCached(string name)
    {
        ModuleInformation? dependency = null;
        lock (s_Cached)
        {
            if (s_Cached.TryGetValue(name, out dependency))
            {
                return dependency;
            }
        }

        if (SearchedModuleCache.TryGetModule(name, out var searched) == false)
        {
            throw new KeyNotFoundException(name);
        }

        var sourcePath = searched.SourcePath;
        var rules = searched.Rules;
        var projectDir = searched.ProjectDirectory;

        string AsFullPath(string currentPath)
        {
            return Path.Combine(sourcePath, currentPath);
        }

        var includePath = Path.Combine(projectDir.Intermediate.Includes, rules.Name);
        var csharpPath = Path.Combine(projectDir.Intermediate.CSharp, rules.Name);

        var includePaths = new List<string>();
        var additionalMacros = new List<string>();
        var apiDescriptions = new List<string>();
        var additionalLibraries = new List<string>();
        var disableWarnings = new List<int>();

        foreach (var dependName in rules.PublicDependencyModuleNames.Concat(rules.PrivateDependencyModuleNames))
        {
            var depend = GetCached(dependName);
            includePaths.AddRange(depend.PublicIncludePaths);
            additionalMacros.AddRange(depend.PublicAdditionalMacros);
            apiDescriptions.AddRange(depend.DependModules);
            apiDescriptions.Add(dependName);
            additionalLibraries.AddRange(depend.PublicAdditionalLibraries);
            disableWarnings.AddRange(depend.PublicDisableWarnings);
        }

        string[] publicIncludePaths = includePaths.Concat(rules.PublicIncludePaths.Select(AsFullPath)).Append(includePath).Distinct().ToArray();
        string[] privateIncludePaths = publicIncludePaths.Concat(rules.PrivateIncludePaths.Select(AsFullPath)).Distinct().ToArray();
        string[] publicAdditionalMacros = additionalMacros.Concat(rules.PublicAdditionalMacros).Distinct().ToArray();
        string[] privateAdditionalMacros = publicAdditionalMacros.Concat(rules.PrivateAdditionalMacros).Distinct().ToArray();
        string[] publicAdditionalLibs = additionalLibraries.Concat(rules.PublicAdditionalLibraries).Distinct().ToArray();
        string[] privateAdditionalLibs = publicAdditionalLibs.Concat(rules.PrivateAdditionalLibraries).Distinct().ToArray();
        int[] publicDisableWarnings = disableWarnings.Concat(rules.PublicDisableWarnings).Distinct().ToArray();
        int[] privateDisableWarnings = publicDisableWarnings.Concat(rules.PrivateDisableWarnings).Distinct().ToArray();

        IEnumerable<string> sourceFiles = Directory
            .GetFiles(sourcePath, "*", SearchOption.AllDirectories)
            .Where(SourceCodeExtensions.IsSourceCode);

        dependency = new ModuleInformation
        {
            Name = name,
            TargetType = TargetType.Module,
            ProjectDir = projectDir,
            SourceFiles = sourceFiles.ToArray(),
            SourcePath = sourcePath,
            GeneratedIncludePath = includePath,
            CSharpPath = csharpPath,
            GeneratedShaderPath = projectDir.Intermediate.Shaders,
            DependModules = apiDescriptions.Distinct().ToArray(),
            PublicIncludePaths = publicIncludePaths,
            PrivateIncludePaths = privateIncludePaths,
            PublicAdditionalMacros = publicAdditionalMacros,
            PrivateAdditionalMacros = privateAdditionalMacros,
            PublicAdditionalLibraries = publicAdditionalLibs,
            PrivateAdditionalLibraries = privateAdditionalLibs,
            PublicDisableWarnings = publicDisableWarnings,
            PrivateDisableWarnings = privateDisableWarnings,
        };

        lock (s_Cached)
        {
            s_Cached.TryAdd(name, dependency);
        }

        return dependency;
    }

    public static bool Contains(string name)
    {
        lock (s_Cached)
        {
            return s_Cached.ContainsKey(name);
        }
    }
}
