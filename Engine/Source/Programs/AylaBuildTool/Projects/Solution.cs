using System.Reflection;
using System.Threading.Tasks;
using Spectre.Console;

namespace AylaEngine;

internal class Solution
{
    public readonly IReadOnlyList<Project> EngineProjects;
    public readonly IReadOnlyList<Project> GameProjects;
    public readonly IReadOnlyList<Project> AllProjects;
    public readonly GroupDescriptor EngineGroup;
    public readonly GroupDescriptor PrimaryGroup;

    private Solution(List<Project> engineProjects, List<Project> gameProjects, GroupDescriptor engineGroup, GroupDescriptor primaryGroup)
    {
        EngineProjects = engineProjects;
        GameProjects = gameProjects;
        AllProjects = gameProjects.Concat(engineProjects).ToArray();
        EngineGroup = engineGroup;
        PrimaryGroup = primaryGroup;
    }

    public Project? FindProject(string name)
    {
        return AllProjects.FirstOrDefault(p => p.Name == name);
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
                AnsiConsole.MarkupLine("[red]Solution not contains depend project '{0}'.[/]", namesArr[i]);
                throw TerminateException.User();
            }

            results[i] = p;
        }

        return results;
    }

    public static async Task<Solution> ScanProjectsAsync(string engineFolder, string? gameFolder, CancellationToken cancellationToken = default)
    {
        List<Task> tasks = new();
        GroupDescriptor engineGroup = GroupDescriptor.FromRoot(engineFolder, true);
        GroupDescriptor primaryGroup = engineGroup;

        List<Project> engineProjects = new();;
        tasks.Add(ScanDirectoryRecursive(engineProjects, engineGroup, Path.Combine(engineFolder, "Source")));
        List<Project> gameProjects = new();
        if (string.IsNullOrEmpty(gameFolder) == false)
        {
            primaryGroup = GroupDescriptor.FromRoot(gameFolder, false);
            tasks.Add(ScanDirectoryRecursive(gameProjects, primaryGroup, Path.Combine(gameFolder, "Source")));
        }

        await Task.WhenAll(tasks);
        engineProjects.Sort((l, r) => l.Decl.Guid.CompareTo(r.Decl.Guid));
        gameProjects.Sort((l, r) => l.Decl.Guid.CompareTo(r.Decl.Guid));
        return new Solution(engineProjects, gameProjects, engineGroup, primaryGroup);

        async Task ScanDirectoryRecursive(IList<Project> results, GroupDescriptor descriptor, string currentDir)
        {
            var directoryName = Path.GetFileName(currentDir);
            var csprojFileName = Path.Combine(currentDir, directoryName + ".csproj");
            if (File.Exists(csprojFileName))
            {
                Project.Declaration declaration = await ConfigureDeclarationAsync(csprojFileName);

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
                    await CSCompiler.CompileToAsync(directoryName, dllFileName, ruleFileName, [typeof(ModuleRules).Assembly.Location], cancellationToken: cancellationToken);
                    File.Copy(ruleFileName, cacheFileName, true);
                    assembly = await Task.Run(() => Assembly.LoadFile(dllFileName), cancellationToken);
                }

                var ruleType = assembly.GetTypes().First(p => p.Name == directoryName);
                Project.Declaration declaration = await ConfigureDeclarationAsync(ruleFileName);

                lock (results)
                {
                    results.Add(new ModuleProject(directoryName, descriptor, currentDir, ruleType, ruleFileName, declaration));
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
                var fileName = Path.GetFileName(ruleFileName);
                var dirName = Path.Combine(descriptor.IntermediateDirectory, "Rules");
                Directory.CreateDirectory(dirName);

                dllFileName = Path.Combine(dirName, fileName + ".dll");
                cacheFileName = Path.Combine(dirName, fileName + ".cache");

                if (File.Exists(cacheFileName) == false)
                {
                    return null;
                }

                if (File.Exists(dllFileName) == false)
                {
                    return null;
                }

                var ruleText = File.ReadAllText(ruleFileName);
                var cacheText = File.ReadAllText(cacheFileName);
                if (ruleText != cacheText)
                {
                    return null;
                }

                return Assembly.LoadFile(dllFileName);
            }
        }

        async Task<Project.Declaration> ConfigureDeclarationAsync(string fileName)
        {
            fileName += ".meta";

            Project.Declaration? declaration = null;
            if (File.Exists(fileName))
            {
                declaration = await MetadataHelper.DeserializeFromFileAsync<Project.Declaration>(fileName, cancellationToken);
            }

            if (declaration is not { IsValid: true })
            {
                declaration = Project.Declaration.New();
                await MetadataHelper.SerializeToFileAsync(declaration, fileName, cancellationToken);
            }

            return declaration;
        }
    }
}
