// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public class TargetInfo
{
    public required BuildConfiguration BuildConfiguration { get; set; }

    public Configuration Configuration => BuildConfiguration.Configuration;

    public TargetPlatform Platform => BuildConfiguration.Platform;
}
