// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.ProjectFiles;

public interface ISolution
{
    Task GenerateProjectFilesAsync(CancellationToken SToken = default);
}
