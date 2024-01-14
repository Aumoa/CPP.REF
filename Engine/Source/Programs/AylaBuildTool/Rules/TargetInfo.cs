// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;

namespace AE.Rules;

public class TargetInfo
{
    public required BuildConfiguration BuildConfiguration { get; set; }

    public Configuration Configuration => BuildConfiguration.Configuration;

    public TargetPlatform Platform => BuildConfiguration.Platform;
}
