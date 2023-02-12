// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Source;

namespace AE.Projects;

public interface ISolution
{
    void AddProject(IProject Project);

    Task GenerateProjectFilesAsync(CancellationToken CToken = default);
}
