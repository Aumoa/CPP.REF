namespace AylaEngine;

internal class Solution
{
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
        var moduleRuleCompiler = new ModuleRuleCompiler(new ModuleRuleCache());
        var materializer = new ProjectMaterializer(metadataStore, moduleRuleCompiler);

        GroupDescriptor engineGroup = GroupDescriptor.FromRoot(engineFolder, true);
        GroupDescriptor primaryGroup = engineGroup;

        var engineCandidatesTask = ProjectScanner.ScanAsync(
            engineGroup,
            Path.Combine(engineFolder, "Source"),
            cancellationToken);
        Task<IReadOnlyList<ProjectCandidate>> gameCandidatesTask = Task.FromResult<IReadOnlyList<ProjectCandidate>>([]);
        if (string.IsNullOrEmpty(gameFolder) == false)
        {
            primaryGroup = GroupDescriptor.FromRoot(gameFolder, false);
            EnsureGameDirectories(primaryGroup);
            gameCandidatesTask = ProjectScanner.ScanAsync(
                primaryGroup,
                Path.Combine(gameFolder, "Source"),
                cancellationToken);
        }

        await Task.WhenAll(engineCandidatesTask, gameCandidatesTask);

        static void EnsureGameDirectories(GroupDescriptor group)
        {
            Directory.CreateDirectory(group.SourceDirectory);
            Directory.CreateDirectory(group.IntermediateDirectory);
            Directory.CreateDirectory(group.BinariesDirectory);
            Directory.CreateDirectory(group.ContentDirectory);
        }

        var engineProjectsTask = materializer.MaterializeAsync(solution, await engineCandidatesTask, cancellationToken);
        var gameProjectsTask = materializer.MaterializeAsync(solution, await gameCandidatesTask, cancellationToken);
        await Task.WhenAll(engineProjectsTask, gameProjectsTask);

        var engineProjects = (await engineProjectsTask).ToList();
        var gameProjects = (await gameProjectsTask).ToList();
        engineProjects.Sort((l, r) => l.Decl.Guid.CompareTo(r.Decl.Guid));
        gameProjects.Sort((l, r) => l.Decl.Guid.CompareTo(r.Decl.Guid));

        solution.Assign(engineProjects.Concat(gameProjects), engineGroup, primaryGroup);
        return solution;
    }
}
