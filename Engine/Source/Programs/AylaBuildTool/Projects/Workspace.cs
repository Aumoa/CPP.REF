// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.Assemblies;
using AE.IO;
using AE.Source;

namespace AE.Projects;

public static class Workspace
{
    private static readonly Dictionary<string, ScriptableAssembly> s_Modules = new();
    private static readonly Dictionary<string, ScriptableAssembly> s_Interops = new();
    private static readonly List<ProjectDirectory> s_ProjectTargets = new();

    public static ProjectDirectory Current { get; private set; }

    public static IEnumerable<ScriptableAssembly> Assemblies => s_Modules.Values.Concat(s_Interops.Values);

    public static IEnumerable<CppAssembly> CppAssemblies => s_Modules.Values.OfType<CppAssembly>();

    public static IEnumerable<CSharpAssembly> CSharpAssemblies => s_Modules.Values.OfType<CSharpAssembly>();

    public static IEnumerable<InteropAssembly> InteropAssemblies => s_Interops.Values.OfType<InteropAssembly>();

    public static ProjectDirectory ProjectTarget => s_ProjectTargets.Last();

    public static FileReference? ProjectFile { get; private set; }

    [MemberNotNullWhen(false, nameof(ProjectFile))]
    public static bool IsTargetEngine => ProjectTarget == Global.EngineDirectory;

    public static bool TryFindModule(string name, [NotNullWhen(true)] out ScriptableAssembly? assembly)
    {
        return s_Modules.TryGetValue(name, out assembly);
    }

    public static void Init(ProjectDirectory target, FileReference? projectFile)
    {
        Current = target;
        ProjectFile = projectFile;

        s_Modules.Clear();
        s_ProjectTargets.Clear();

        IEnumerable<ProjectDirectory> targetProjects = target == Global.EngineDirectory ? new[] { Global.EngineDirectory } : new[] { Global.EngineDirectory, target };
        foreach (var targetProject in targetProjects)
        {
            InternalCollect(targetProject, targetProject.Source.Root);
        }

        s_ProjectTargets.AddRange(targetProjects);

        if (Workspace.IsTargetEngine == false)
        {
            if (projectFile == null)
            {
                throw new ArgumentNullException(nameof(projectFile));
            }
        }
    }

    public static async Task InitAssembliesAsync(CancellationToken cancellationToken = default)
    {
        List<Task> tasks = new();
        foreach (var module in Assemblies)
        {
            tasks.Add(module.CompileScriptableAssemblyAsync(cancellationToken));
        }

        await Task.WhenAll(tasks);
    }

    public static void Cleanup()
    {
        foreach (var projectTarget in s_ProjectTargets)
        {
            projectTarget.Cleanup();
        }
    }

    private static void InternalCollect(ProjectDirectory target, DirectoryReference directory, CancellationToken cancellationToken = default)
    {
        cancellationToken.ThrowIfCancellationRequested();

        var sourceFiles = directory.GetFiles();

        bool hasModule = false;
        foreach (var sourceFile in sourceFiles)
        {
            if (sourceFile.FileName.EndsWith(".Module.cs"))
            {
                var assembly = new CppAssembly(target, sourceFile, sourceFile.FileName[..^".Module.cs".Length]);
                s_Modules.Add(assembly.Name, assembly);
                hasModule = true;
            }

            if (sourceFile.FileName.EndsWith(".Interop.cs"))
            {
                var assembly = new InteropAssembly(target, sourceFile, sourceFile.FileName[..^".Interop.cs".Length]);
                s_Interops.Add(assembly.Name, assembly);
                hasModule = true;
            }
        }

        if (hasModule == false)
        {
            var csharpModule = sourceFiles.FirstOrDefault(p => p.Extensions.EndsWith("csproj", StringComparison.OrdinalIgnoreCase));
            if (csharpModule != null)
            {
                var assembly = new CSharpAssembly(target, csharpModule);
                s_Modules.Add(assembly.Name, assembly);
                return;
            }

            foreach (var subdir in directory.GetDirectories())
            {
                InternalCollect(target, subdir, cancellationToken);
            }
        }
    }
}
