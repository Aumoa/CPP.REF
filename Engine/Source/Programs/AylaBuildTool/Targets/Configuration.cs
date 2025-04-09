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
    public static bool IsOptimized(this Configuration configuration)
    {
        return configuration == Configuration.Shipping || configuration == Configuration.Development;
    }
}