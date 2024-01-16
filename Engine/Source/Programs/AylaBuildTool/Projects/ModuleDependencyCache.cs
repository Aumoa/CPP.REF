// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Data;

using AE.Assemblies;
using AE.Exceptions;
using AE.Extensions;
using AE.IO;
using AE.Rules;

namespace AE.Projects;

public static class ModuleDependencyCache
{
    private static readonly Stack<Dictionary<string, ModuleInformation>> s_Stack = new();
    private static Dictionary<string, ModuleInformation> s_Cached = new();

    public static IEnumerable<ModuleInformation> Modules => s_Cached.Values;

    public static void Clear()
    {
        s_Cached = new();
    }

    public static void Push()
    {
        s_Stack.Push(s_Cached);
        s_Cached = new();
    }

    public static void Pop()
    {
        s_Cached = s_Stack.Pop();
    }

    public static void BuildCache(string name, string source) => GetCached(name, source);

    public static ModuleInformation GetCached(string name, string source)
    {
        ModuleInformation? dependency = null;
        lock (s_Cached)
        {
            if (s_Cached.TryGetValue(name, out dependency))
            {
                return dependency;
            }
        }

        if (Workspace.TryFindModule(name, out var assembly) == false || assembly is not CppAssembly cppAssembly)
        {
            string message;
            if (string.IsNullOrEmpty(source))
            {
                message = CoreStrings.Errors.ModuleNotFound(name);
            }
            else
            {
                message = CoreStrings.Errors.DependencyModuleNotFound(source, name);
            }
            throw new TerminateException(KnownErrorCode.ModuleNotFound, message);
        }

        var sourcePath = cppAssembly.SourceDirectory;
        var rules = cppAssembly.Rules;
        var projectDir = cppAssembly.ProjectDirectory;

        string AsFullPath(string currentPath)
        {
            return Path.Combine(sourcePath, currentPath);
        }

        var includePath = projectDir.Intermediate.Includes.GetChild(name);
        var csharpPath = Path.Combine(projectDir.Intermediate.CSharp, rules.Name);

        var includePaths = new List<string>();
        var additionalMacros = new List<string>();
        var apiDescriptions = new List<string>();
        var additionalLibraries = new List<string>();
        var disableWarnings = new List<int>();

        foreach (var dependName in rules.PublicDependencyModuleNames.Concat(rules.PrivateDependencyModuleNames))
        {
            var depend = GetCached(dependName, name);
            includePaths.AddRange(depend.PublicIncludePaths);
            additionalMacros.AddRange(depend.PublicAdditionalMacros);
            apiDescriptions.AddRange(depend.DependModules);
            apiDescriptions.Add(dependName);
            additionalLibraries.AddRange(depend.PublicAdditionalLibraries);
            disableWarnings.AddRange(depend.PublicDisableWarnings);
        }

        string[] publicIncludePaths = includePaths.Concat(rules.PublicIncludePaths.Select(AsFullPath)).Append(includePath.Value).Distinct().ToArray();
        string[] privateIncludePaths = publicIncludePaths.Concat(rules.PrivateIncludePaths.Select(AsFullPath)).Distinct().ToArray();
        string[] publicAdditionalMacros = additionalMacros.Concat(rules.PublicAdditionalMacros).Distinct().ToArray();
        string[] privateAdditionalMacros = publicAdditionalMacros.Concat(rules.PrivateAdditionalMacros).Distinct().ToArray();
        string[] publicAdditionalLibs = additionalLibraries.Concat(rules.PublicAdditionalLibraries).Distinct().ToArray();
        string[] privateAdditionalLibs = publicAdditionalLibs.Concat(rules.PrivateAdditionalLibraries).Distinct().ToArray();
        int[] publicDisableWarnings = disableWarnings.Concat(rules.PublicDisableWarnings).Distinct().ToArray();
        int[] privateDisableWarnings = publicDisableWarnings.Concat(rules.PrivateDisableWarnings).Distinct().ToArray();

        IEnumerable<FileReference> sourceFiles = sourcePath
            .GetFiles(recursive: true)
            .Where(p => SourceCodeUtility.IsSourceFile(p));

        dependency = new ModuleInformation
        {
            Name = name,
            TargetType = rules.Type,
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
