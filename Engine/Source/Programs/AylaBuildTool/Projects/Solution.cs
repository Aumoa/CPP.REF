namespace AylaEngine;

internal class Solution
{
    public readonly IReadOnlyList<Project> EngineProjects;
    public readonly IReadOnlyList<Project> GameProjects;
    public readonly IReadOnlyList<Project> AllProjects;
    public readonly GroupDescriptor PrimaryGroup;

    private Solution(List<Project> engineProjects, List<Project> gameProjects, GroupDescriptor primaryGroup)
    {
        EngineProjects = engineProjects;
        GameProjects = gameProjects;
        AllProjects = gameProjects.Concat(engineProjects).ToArray();
        PrimaryGroup = primaryGroup;
    }

    public static async Task<Solution> ScanProjectsAsync(string engineFolder, string? gameFolder, CancellationToken cancellationToken = default)
    {
        List<Task> tasks = new();
        GroupDescriptor primaryGroup = GroupDescriptor.FromRoot(engineFolder);

        List<Project> engineProjects = new();;
        tasks.Add(ScanDirectoryRecursive(engineProjects, primaryGroup, Path.Combine(engineFolder, "Source")));
        List<Project> gameProjects = new();
        if (gameFolder != null)
        {
            primaryGroup = GroupDescriptor.FromRoot(gameFolder);
            tasks.Add(ScanDirectoryRecursive(gameProjects, primaryGroup, Path.Combine(gameFolder, "Source")));
        }

        await Task.WhenAll(tasks);
        return new Solution(engineProjects, gameProjects, primaryGroup);

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
                var ruleAssembly = await CSCompiler.CompileAsync(directoryName, ruleFileName, [typeof(ModuleRules).Assembly.Location], cancellationToken: cancellationToken);
                var ruleType = ruleAssembly.GetTypes().First(p => p.Name == directoryName);

                Project.Declaration declaration = await ConfigureDeclarationAsync(ruleFileName);

                lock (results)
                {
                    results.Add(new ModuleProject(directoryName, descriptor, currentDir, ruleType, declaration));
                }
                return;
            }

            List<Task> innerTasks = new();
            foreach (var subDir in Directory.GetDirectories(currentDir, "*", SearchOption.TopDirectoryOnly))
            {
                innerTasks.Add(ScanDirectoryRecursive(results, descriptor, subDir));
            }

            await Task.WhenAll(innerTasks);
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
