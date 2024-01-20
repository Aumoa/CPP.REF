// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.BuildSettings;

public enum Configuration
{
    Debug,
    DebugGame,
    Development,
    Shipping
}

public static class ConfigurationExtensions
{
    public static bool IsDebug(this Configuration InConfig)
    {
        return InConfig is Configuration.Debug or Configuration.DebugGame;
    }

    public static bool IsRelease(this Configuration InConfig)
    {
        return !InConfig.IsDebug();
    }
}
