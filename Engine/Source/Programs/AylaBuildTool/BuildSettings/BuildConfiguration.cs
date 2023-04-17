// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Rules;

namespace AE.BuildSettings;

public class BuildConfiguration : SerializableRule
{
    public Configuration Configuration { get; set; } = Configuration.Debug;

    public TargetPlatform Platform { get; set; } = new();

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
