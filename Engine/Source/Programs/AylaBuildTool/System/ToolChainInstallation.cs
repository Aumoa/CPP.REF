// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.System;

public abstract class ToolChainInstallation
{
    public abstract string BaseDirectory { get; }

    public abstract Version InstallVersion { get; }
}
