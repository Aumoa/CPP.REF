namespace AylaEngine;

internal sealed class SolutionLoader
{
    private readonly ProjectScanner m_ProjectScanner;
    private readonly ProjectMaterializer m_ProjectMaterializer;

    public SolutionLoader(ProjectScanner projectScanner, ProjectMaterializer projectMaterializer)
    {
        m_ProjectScanner = projectScanner;
        m_ProjectMaterializer = projectMaterializer;
    }

    public static SolutionLoader CreateDefault()
    {
        var metadataStore = new ProjectMetadataStore();
        var moduleRuleCompiler = new ModuleRuleCompiler(new ModuleRuleCache());
        var materializer = new ProjectMaterializer(metadataStore, moduleRuleCompiler);

        return new SolutionLoader(new ProjectScanner(), materializer);
    }

    public async Task<Solution> LoadAsync(
        string engineFolder,
        string? projectFile,
        CancellationToken cancellationToken)
    {
        var normalizedProjectFile = projectFile == null ? null : Path.GetFullPath(projectFile);
        string? gameFolder = normalizedProjectFile == null ? null : Path.GetDirectoryName(normalizedProjectFile);

        GroupDescriptor engineGroup = GroupDescriptor.FromRoot(engineFolder, true);
        GroupDescriptor primaryGroup = engineGroup;

        var engineCandidatesTask = m_ProjectScanner.ScanAsync(
            engineGroup,
            Path.Combine(engineFolder, "Source"),
            cancellationToken);
        Task<IReadOnlyList<ProjectCandidate>> gameCandidatesTask = Task.FromResult<IReadOnlyList<ProjectCandidate>>([]);
        if (string.IsNullOrEmpty(gameFolder) == false)
        {
            primaryGroup = GroupDescriptor.FromRoot(gameFolder, false);
            EnsureGameDirectories(primaryGroup);
            gameCandidatesTask = m_ProjectScanner.ScanAsync(
                primaryGroup,
                Path.Combine(gameFolder, "Source"),
                cancellationToken);
        }

        await Task.WhenAll(engineCandidatesTask, gameCandidatesTask);

        var engineProjectsTask = m_ProjectMaterializer.MaterializeAsync(await engineCandidatesTask, cancellationToken);
        var gameProjectsTask = m_ProjectMaterializer.MaterializeAsync(await gameCandidatesTask, cancellationToken);
        await Task.WhenAll(engineProjectsTask, gameProjectsTask);

        var engineProjects = SortProjectsByGuid(await engineProjectsTask);
        var gameProjects = SortProjectsByGuid(await gameProjectsTask);

        return new Solution(
            normalizedProjectFile,
            engineProjects.Concat(gameProjects),
            engineGroup,
            primaryGroup);
    }

    private static void EnsureGameDirectories(GroupDescriptor group)
    {
        Directory.CreateDirectory(group.SourceDirectory);
        Directory.CreateDirectory(group.IntermediateDirectory);
        Directory.CreateDirectory(group.BinariesDirectory);
        Directory.CreateDirectory(group.ContentDirectory);
    }

    private static List<Project> SortProjectsByGuid(IEnumerable<Project> projects)
    {
        var sortedProjects = projects.ToList();
        sortedProjects.Sort((l, r) => l.Decl.Guid.CompareTo(r.Decl.Guid));
        return sortedProjects;
    }
}
