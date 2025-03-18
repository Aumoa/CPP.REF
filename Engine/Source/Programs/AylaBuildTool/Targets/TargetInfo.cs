namespace AylaEngine;

internal record TargetInfo : ITargetInfo
{
    public PlatformInfo Platform { get; init; } = PlatformInfo.Win64;

    public Configuration Config { get; init; } = Configuration.Shipping;

    public bool Editor { get; init; }

    public static readonly TargetInfo Default = new()
    {
        Platform = PlatformInfo.Win64,
        Config = Configuration.Shipping,
        Editor = false
    };

    public static IEnumerable<TargetInfo> GetAllTargets()
    {
        return InnerGet(PlatformInfo.Win64, true)
            .Concat(InnerGet(PlatformInfo.Win64, false));

        IEnumerable<TargetInfo> InnerGet(PlatformInfo platform, bool editor)
        {
            foreach (var config in Enum.GetValues<Configuration>())
            {
                yield return new TargetInfo
                {
                    Platform = platform,
                    Config = config,
                    Editor = editor
                };
            }
        }
    }
}
