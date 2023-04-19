// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Misc;
using AE.Platform;
using AE.Rules;

namespace AE.BuildSettings;

public class TargetPlatform
{
    public string TargetName { get; set; } = string.Empty;

    public PlatformGroup Group { get; set; } = new();

    public Architecture Architecture { get; set; }

    public override string ToString()
    {
        return TargetName;
    }

    public static readonly TargetPlatform Win64 = new()
    {
        TargetName = "Win64",
        Group = PlatformGroup.Windows,
        Architecture = Architecture.x64
    };

    public static readonly TargetPlatform Linux = new()
    {
        TargetName = "Linux",
        Group = PlatformGroup.Linux,
        Architecture = Architecture.x64
    };
}
