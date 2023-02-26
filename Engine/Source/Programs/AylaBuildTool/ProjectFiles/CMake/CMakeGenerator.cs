// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Projects;

namespace AE.ProjectFiles.CMake;

public static class CMakeGenerator
{
    public static async Task<CMakeSolution> GenerateSolutionAsync(Workspace InWorkspace, string? ProjectDir = null, CancellationToken CToken = default)
    {
        CMakeSolution Solution = new(InWorkspace);

        foreach (var Project in InWorkspace.CXXProjects)
        {
            Solution.AddProject(new CMakeCXXProject(Project));
        }

        await Solution.GenerateProjectFilesAsync(ProjectDir ?? InWorkspace.ProjectPath, CToken);
        return Solution;
    }
}
