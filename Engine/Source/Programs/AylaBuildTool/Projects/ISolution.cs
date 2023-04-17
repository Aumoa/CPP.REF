// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Projects;

public interface ISolution
{
    Task GenerateProjectFilesAsync(CancellationToken SToken = default);
}
