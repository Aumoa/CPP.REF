// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.System;

namespace AE.ProjectFiles;

public interface ISolution
{
    Task GenerateProjectFilesAsync(CancellationToken cancellationToken = default);
}
