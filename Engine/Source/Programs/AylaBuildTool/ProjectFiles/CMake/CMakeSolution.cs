// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Projects;

namespace AE.ProjectFiles.CMake;

public class CMakeSolution : ISolution
{
    private readonly Workspace TargetWorkspace;

    public List<CMakeCXXProject> CXXProjects { get; init; } = new();

    public CMakeSolution(Workspace InWorkspace)
    {
        TargetWorkspace = InWorkspace;
    }

    public void AddProject(IProject Project)
    {
        if (Project is CMakeCXXProject CXProject)
        {
            CXXProjects.Add(CXProject);
        }
        else
        {
            throw new InvalidOperationException(CoreStrings.Errors.InvalidProjectFormat);
        }
    }

    public async Task GenerateProjectFilesAsync(string _, CancellationToken CToken = default)
    {
        foreach (var Project in CXXProjects)
        {
            await Project.GenerateCMakeProject(TargetWorkspace, CToken);
        }
    }
}
