// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.BuildSettings;

public record TargetPlatform
{
    private readonly string TargetName;

    private TargetPlatform(string TargetName)
    {
        this.TargetName = TargetName;
    }

    public override string ToString() => TargetName;

    public static readonly TargetPlatform Win64 = new("Win64")
    {
    };
}
