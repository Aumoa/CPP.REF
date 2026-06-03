namespace AylaEngine;

internal class Solution
{
    public readonly string? ProjectFile;

    public IReadOnlyList<Project> Projects { get; private set; } = null!;

    public GroupDescriptor EngineGroup { get; private set; } = null!;

    public GroupDescriptor PrimaryGroup { get; private set; } = null!;

    internal Solution(string? projectFile)
    {
        ProjectFile = projectFile;
    }

    internal void Assign(IEnumerable<Project> projects, GroupDescriptor engineGroup, GroupDescriptor primaryGroup)
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
        var metadataStore = new ProjectMetadataStore();
        var moduleRuleCompiler = new ModuleRuleCompiler(new ModuleRuleCache());
        var materializer = new ProjectMaterializer(metadataStore, moduleRuleCompiler);
        var loader = new SolutionLoader(new ProjectScanner(), materializer);

        return await loader.LoadAsync(engineFolder, projectFile, cancellationToken);
    }
}
