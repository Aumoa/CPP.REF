namespace AylaEngine;

public abstract record CSReference(string Include) : CSElement
{
    public abstract string ReferencedAssemblyPath(CSCondition? condition, Dictionary<string, CSProject> virtualProjects, string projectDirectory, HashSet<string> referencedAssemblies);
}
