// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Projects;

public interface ISolution
{
    void AddProject(IProject Project);

    Task GenerateProjectFilesAsync(string ProjectPath, CancellationToken CToken = default);
}
