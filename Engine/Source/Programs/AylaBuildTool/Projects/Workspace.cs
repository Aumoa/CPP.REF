// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.Assemblies;
using AE.IO;
using AE.Source;

namespace AE.Projects;

public static class Workspace
{
    private static readonly Dictionary<string, ScriptableAssembly> s_Modules = new();
    private static readonly List<ProjectDirectory> s_ProjectTargets = new();

    public static ProjectDirectory Current { get; private set; }

    public static IEnumerable<ScriptableAssembly> Assemblies => s_Modules.Values;

    public static IEnumerable<CppAssembly> CppAssemblies => Assemblies.OfType<CppAssembly>();

    public static IEnumerable<CSharpAssembly> CSharpAssemblies => Assemblies.OfType<CSharpAssembly>();

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
        var csharpModule = sourceFiles.FirstOrDefault(p => p.Extensions.Equals("csproj", StringComparison.OrdinalIgnoreCase));
        if (csharpModule != null)
        {
            var assembly = new CSharpAssembly(target, csharpModule);
            s_Modules.Add(assembly.Name, assembly);
            return;
        }

        bool hasModule = false;
        foreach (var sourceFile in sourceFiles)
        {
            if (sourceFile.FileName.EndsWith(".Module.cs"))
            {
                var assembly = new CppAssembly(target, sourceFile);
                s_Modules.Add(assembly.Name, assembly);
                hasModule = true;
            }
        }

        if (hasModule == false)
        {
            foreach (var subdir in directory.GetDirectories())
            {
                InternalCollect(target, subdir, cancellationToken);
            }
        }
    }
}
