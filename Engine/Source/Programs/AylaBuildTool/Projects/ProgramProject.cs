namespace AylaEngine;

internal class ProgramProject(string name, GroupDescriptor descriptor, Project.Declaration declaration, string projectFilePath) : Project(name, descriptor, declaration)
{
    public readonly string ProjectFilePath = projectFilePath;
}
