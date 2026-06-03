namespace AylaEngine;

internal class Solution
{
    public readonly string? ProjectFile;

    public IReadOnlyList<Project> Projects { get; }

    public GroupDescriptor EngineGroup { get; }

    public GroupDescriptor PrimaryGroup { get; }

    internal Solution(
        string? projectFile,
        IEnumerable<Project> projects,
        GroupDescriptor engineGroup,
        GroupDescriptor primaryGroup)
    {
        ProjectFile = projectFile;
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
}
