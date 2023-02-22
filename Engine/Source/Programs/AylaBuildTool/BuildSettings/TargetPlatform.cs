// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.BuildSettings;

public record TargetPlatform
{
    private readonly string TargetName;

    public PlatformGroup Group { get; init; } = null!;

    private TargetPlatform(string TargetName)
    {
        this.TargetName = TargetName;
    }

    public override string ToString() => TargetName;

    public static readonly TargetPlatform Win64 = new("Win64")
    {
        Group = PlatformGroup.Windows
    };

    public static readonly TargetPlatform Linux = new("Linux")
    {
        Group = PlatformGroup.Linux
    };
}
