namespace AylaEngine;

public abstract record CSReferenceBase(string Include) : CSElement
{
    public abstract string ReferencedAssemblyPath(CSCondition? condition, string projectDirectory);
}
