// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Platform;

namespace AE.BuildSettings;

public record TargetPlatform
{
    private readonly string TargetName;

    public PlatformGroup Group { get; init; } = null!;

    public Architecture Architecture { get; init; }

    private TargetPlatform(string TargetName)
    {
        this.TargetName = TargetName;
    }

    public override string ToString() => TargetName;

    public static readonly TargetPlatform Win64 = new("Win64")
    {
        Group = PlatformGroup.Windows,
        Architecture = Architecture.x64
    };

    public static readonly TargetPlatform Linux = new("Linux")
    {
        Group = PlatformGroup.Linux,
        Architecture = Architecture.x64
    };
}
