namespace AylaEngine;

internal record TargetInfo : ITargetInfo
{
    public PlatformInfo Platform { get; init; } = PlatformInfo.Win64;

    public static readonly BuildConfiguration Default = new()
    {
        Platform = PlatformInfo.Win64
    };
}
