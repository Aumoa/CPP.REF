namespace AylaEngine;

public abstract record CSReference(string Include) : CSElement
{
    public abstract string ReferencedAssemblyPath(CSCondition? condition, string projectDirectory, HashSet<string> referencedAssemblies);
}
