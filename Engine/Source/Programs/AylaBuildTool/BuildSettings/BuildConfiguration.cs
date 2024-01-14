// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.BuildSettings;

public class BuildConfiguration
{
    public Configuration Configuration { get; set; } = Configuration.Debug;

    public required TargetPlatform Platform { get; set; }

    public static readonly Configuration[] Configurations = new[]
    {
        Configuration.Debug,
        Configuration.DebugGame,
        Configuration.Development,
        Configuration.Shipping
    };

    public static readonly TargetPlatform[] Platforms = new[]
    {
        TargetPlatform.Win64,
        TargetPlatform.Linux
    };

    public static void ForEach(Action<Configuration, bool, TargetPlatform> InAction)
    {
        foreach (var Platform in Platforms)
        {
            foreach (var Configuration in Configurations)
            {
                InAction(Configuration, false, Platform);
                if (Configuration != Configuration.Shipping)
                {
                    // Contains editor build.
                    InAction(Configuration, true, Platform);
                }
            }
        }
    }
}
