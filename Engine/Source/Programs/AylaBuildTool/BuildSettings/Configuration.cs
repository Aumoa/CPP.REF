// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

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
