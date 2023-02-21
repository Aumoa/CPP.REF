// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.BuildSettings;

public struct BuildConfiguration
{
    public Configuration Configuration;

    public TargetPlatform Platform;

    public static readonly Configuration[] Configurations = new[] { Configuration.Debug, Configuration.DebugGame, Configuration.Development, Configuration.Shipping };

    public static readonly TargetPlatform[] Platforms = new[] { TargetPlatform.Win64 };

    public static void ForEach(Action<Configuration, TargetPlatform> InAction)
    {
        foreach (var Platform in Platforms)
        {
            foreach (var Configuration in Configurations)
            {
                InAction(Configuration, Platform);
            }
        }
    }
}
