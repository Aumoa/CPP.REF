// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Projects;

namespace AE.ProjectFiles.VisualStudio;

public static class VisualStudioGenerator
{
    public static async Task GenerateSolutionAsync(Workspace InWorkspace, CancellationToken CToken = default)
    {
        VisualStudioSolution Solution = new(InWorkspace);

        foreach (var Project in InWorkspace.CXXProjects)
        {
            Solution.AddProject(new VisualCXXProject(Project));
        }

        foreach (var Project in InWorkspace.CSProjects)
        {
            Solution.AddProject(new VisualCSharpProject(Project.ProjectFile, "Programs"));
        }

        await Solution.GenerateProjectFilesAsync(CToken);
    }
}
