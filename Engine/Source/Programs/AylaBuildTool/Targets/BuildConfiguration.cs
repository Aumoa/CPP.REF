namespace AylaEngine;

public record BuildConfiguration
{
    public PlatformInfo Platform { get; init; } = PlatformInfo.Win64;

    public Configuration Config { get; init; } = Configuration.Shipping;

    public bool Editor { get; init; }

    public static readonly BuildConfiguration Default = new()
    {
        Platform = PlatformInfo.Win64,
        Config = Configuration.Shipping,
        Editor = false
    };

    public static IEnumerable<BuildConfiguration> GetConfigurations()
    {
        return InnerGet(PlatformInfo.Win64, false)
            .Concat(InnerGet(PlatformInfo.Win64, true));

        IEnumerable<BuildConfiguration> InnerGet(PlatformInfo platform, bool editor)
        {
            foreach (var config in Enum.GetValues<Configuration>())
            {
                yield return new BuildConfiguration
                {
                    Platform = platform,
                    Config = config,
                    Editor = editor
                };
            }
        }
    }
}
