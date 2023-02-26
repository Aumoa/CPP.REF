// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Platform;

namespace AE.System;

public abstract class ToolChainInstallation
{
    public abstract string BaseDirectory { get; }

    public abstract Version InstallVersion { get; }

    public override string ToString()
    {
        return $"{InstallVersion} in {BaseDirectory}";
    }

    public abstract string[] GetRequiredExecutablePaths(Architecture TargetArchitecture);

    public abstract string[] GetRequiredLibraryPaths(Architecture TargetArchitecture);

    public abstract string[] GetRequiredIncludePaths(Architecture TargetArchitecture);
}
