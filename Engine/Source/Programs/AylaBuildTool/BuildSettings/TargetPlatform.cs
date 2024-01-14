// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Platform;

namespace AE.BuildSettings;

public class TargetPlatform
{
    public readonly string TargetName;

    public PlatformGroup Group { get; set; } = new();

    public Architecture Architecture { get; set; }

    public override string ToString()
    {
        return TargetName;
    }

    private static readonly Dictionary<string, TargetPlatform> Platforms = new();

    public TargetPlatform(string InTargetName)
    {
        TargetName = InTargetName;
        Platforms.Add(TargetName, this);
    }

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

    public static TargetPlatform Current
    {
        get
        {
            if (Environment.OSVersion.Platform == PlatformID.Win32NT)
            {
                return Win64;
            }

            throw new NotSupportedException("Not supported platform detected. Please contact to developer.");
        }
    }

    public static TargetPlatform FromTargetName(string InTargetName)
    {
        return Platforms[InTargetName];
    }
}
