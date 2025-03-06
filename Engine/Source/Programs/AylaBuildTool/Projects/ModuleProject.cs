namespace AylaEngine;

internal class ModuleProject(string name, GroupDescriptor descriptor, string sourceDirectory, Type ruleType, Project.Declaration declaration) : Project(name, descriptor, declaration)
{
    public readonly Type RuleType = ruleType;
    public readonly string SourceDirectory = sourceDirectory;
}
