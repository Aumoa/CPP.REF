using System.Diagnostics;
using System.Reflection;
using Microsoft.CodeAnalysis;

namespace AylaEngine;

internal class Solution
{
    private static readonly CSProject ProjectTemplate = new("Microsoft.NET.Sdk",
        [new CSPropertyGroup(
            null,
            OutputKind.DynamicallyLinkedLibrary,
            CSTargetFramework.Net0900,
            true,
            NullableContextOptions.Enable,
            null,
            null,
            null,
            null,
            null,
            null,
            null,
            [],
            [],
            null,
            null,
            [],
            null
            )],
        [new CSItemGroup(
            null,
            [new CSFileReference("AylaBuildRules", typeof(ModuleRules).Assembly.Location)],
            [],
            []
            )],
        null);

    public readonly string? ProjectFile;

    public IReadOnlyList<Project> Projects { get; private set; } = null!;

    public GroupDescriptor EngineGroup { get; private set; } = null!;

    public GroupDescriptor PrimaryGroup { get; private set; } = null!;

    private Solution(string? projectFile)
    {
        ProjectFile = projectFile;
    }

    private void Assign(IEnumerable<Project> projects, GroupDescriptor engineGroup, GroupDescriptor primaryGroup)
    {
        Projects = projects.ToArray();
        EngineGroup = engineGroup;
        PrimaryGroup = primaryGroup;
    }

    public Project? FindProject(string name)
    {
        return Projects.FirstOrDefault(p => p.Name == name);
    }

    public Project[] FindDepends(params IEnumerable<string> names)
    {
        var namesArr = names.ToArray();
        var results = new Project[namesArr.Length];
        for (int i = 0; i < namesArr.Length; ++i)
        {
            var p = FindProject(namesArr[i]);
            if (p == null)
            {
                Console.Error.WriteLine("Solution not contains depend project '{0}'.", namesArr[i]);
                throw TerminateException.User();
            }

            results[i] = p;
        }

        return results;
    }

    public static async Task<Solution> ScanProjectsAsync(string engineFolder, string? projectFile, CancellationToken cancellationToken = default)
    {
        var solution = new Solution(projectFile == null ? null : Path.GetFullPath(projectFile));
        string? gameFolder = solution.ProjectFile == null ? null : Path.GetDirectoryName(solution.ProjectFile);
        var metadataStore = new ProjectMetadataStore();

        List<Task> tasks = new();
        GroupDescriptor engineGroup = GroupDescriptor.FromRoot(engineFolder, true);
        GroupDescriptor primaryGroup = engineGroup;

        List<Project> engineProjects = new();;
        tasks.Add(ScanDirectoryRecursive(engineProjects, primaryGroup, Path.Combine(engineFolder, "Source")));
        List<Project> gameProjects = new();
        if (string.IsNullOrEmpty(gameFolder) == false)
        {
            primaryGroup = GroupDescriptor.FromRoot(gameFolder, false);
            EnsureGameDirectories(primaryGroup);
            tasks.Add(ScanDirectoryRecursive(gameProjects, primaryGroup, Path.Combine(gameFolder, "Source")));
        }

        await Task.WhenAll(tasks);

        static void EnsureGameDirectories(GroupDescriptor group)
        {
            Directory.CreateDirectory(group.SourceDirectory);
            Directory.CreateDirectory(group.IntermediateDirectory);
            Directory.CreateDirectory(group.BinariesDirectory);
            Directory.CreateDirectory(group.ContentDirectory);
        }
        engineProjects.Sort((l, r) => l.Decl.Guid.CompareTo(r.Decl.Guid));
        gameProjects.Sort((l, r) => l.Decl.Guid.CompareTo(r.Decl.Guid));

        solution.Assign(engineProjects.Concat(gameProjects), engineGroup, primaryGroup);
        return solution;

        async Task ScanDirectoryRecursive(IList<Project> results, GroupDescriptor descriptor, string currentDir)
        {
            var directoryName = Path.GetFileName(currentDir);
            var csprojFileName = Path.Combine(currentDir, directoryName + ".csproj");
            if (File.Exists(csprojFileName))
            {
                Project.Declaration declaration = await metadataStore.GetOrCreateProjectDeclarationAsync(csprojFileName, cancellationToken);

                lock (results)
                {
                    results.Add(new ProgramProject(directoryName, descriptor, declaration, csprojFileName));
                }
                return;
            }

            var ruleFileName = Path.Combine(currentDir, directoryName + ".Module.cs");
            if (File.Exists(ruleFileName))
            {
                var assembly = FindCachedAssembly(ruleFileName, out var dllFileName, out var cacheFileName);
                if (assembly == null)
                {
                    var projectName = Path.GetFileNameWithoutExtension(dllFileName);
                    var csproj = ProjectTemplate with
                    {
                        PropertyGroups = [ProjectTemplate.PropertyGroup with
                        {
                            OutputPath = Path.GetDirectoryName(dllFileName),
                            AssemblyName = projectName
                        }]
                    };

                    CSSourceCode[] sourceCodes = [CSSourceCode.FromFile(ruleFileName)];

                    try
                    {
                        await CSCompiler.CompileAsAsync(sourceCodes, csproj, [], currentDir, projectName, cancellationToken);
                        await File.WriteAllTextAsync(cacheFileName, CreateRuleCacheText(ruleFileName), cancellationToken);
                        assembly = await Task.Run(() => Assembly.LoadFile(dllFileName), cancellationToken);
                    }
                    catch (CSCompilerError e)
                    {
                        Console.Error.WriteLine(e.Message);
                        throw TerminateException.User();
                    }
                }

                var className = directoryName.Replace('.', '_');
                var ruleType = assembly.GetTypes().First(p => p.Name == className);
                ModuleProject.ModuleDeclaration declaration = await metadataStore.GetOrCreateModuleDeclarationAsync(ruleFileName, cancellationToken);

                lock (results)
                {
                    results.Add(new ModuleProject(solution, directoryName, descriptor, currentDir, ruleType, ruleFileName, declaration));
                }
                return;
            }

            List<Task> innerTasks = new();
            foreach (var subDir in Directory.GetDirectories(currentDir, "*", SearchOption.TopDirectoryOnly))
            {
                innerTasks.Add(ScanDirectoryRecursive(results, descriptor, subDir));
            }

            await Task.WhenAll(innerTasks);

            return;

            Assembly? FindCachedAssembly(string ruleFileName, out string dllFileName, out string cacheFileName)
            {
                var fileName = Path.GetFileName(ruleFileName).Replace('.', '_');
                var dirName = Path.Combine(descriptor.IntermediateDirectory, "Rules");
                Directory.CreateDirectory(dirName);

                dllFileName = Path.GetFullPath(Path.Combine(dirName, fileName + ".dll"));
                cacheFileName = Path.GetFullPath(Path.Combine(dirName, fileName + ".cache"));

                if (File.Exists(cacheFileName) == false)
                {
                    return null;
                }

                if (File.Exists(dllFileName) == false)
                {
                    return null;
                }

                var ruleText = CreateRuleCacheText(ruleFileName);
                var cacheText = File.ReadAllText(cacheFileName);
                if (ruleText != cacheText)
                {
                    return null;
                }

                return Assembly.LoadFile(dllFileName);
            }
        }

        static string CreateRuleCacheText(string ruleFileName)
        {
            var rulesAssemblyFileName = typeof(ModuleRules).Assembly.Location;
            var rulesAssemblyWriteTime = File.GetLastWriteTimeUtc(rulesAssemblyFileName).ToBinary();
            var ruleText = File.ReadAllText(ruleFileName);

            return string.Join('\n',
            [
                "AylaBuildRulesCacheVersion=1",
                $"RulesAssembly={rulesAssemblyFileName}",
                $"RulesAssemblyWriteTimeUtc={rulesAssemblyWriteTime}",
                ruleText
            ]);
        }

    }
}
