namespace AylaEngine;

public enum Configuration
{
    Debug,
    DebugGame,
    Development,
    Shipping
}

public enum OptimizationMode
{
    Debug,
    Release
}

public enum RuntimeLibraryMode
{
    Debug,
    Release
}

public enum AssertionMode
{
    Enabled,
    Disabled
}

public enum AbiMode
{
    Debug,
    Release
}

public readonly record struct BuildConfigurationProfile(
    string Name,
    OptimizationMode Optimization,
    RuntimeLibraryMode RuntimeLibrary,
    AssertionMode Assertions,
    AbiMode Abi)
{
    public static BuildConfigurationProfile Debug { get; } = new(
        nameof(Debug),
        OptimizationMode.Debug,
        RuntimeLibraryMode.Debug,
        AssertionMode.Enabled,
        AbiMode.Debug);

    public static BuildConfigurationProfile DebugGame { get; } = new(
        nameof(DebugGame),
        OptimizationMode.Debug,
        RuntimeLibraryMode.Release,
        AssertionMode.Enabled,
        AbiMode.Release);

    public static BuildConfigurationProfile Development { get; } = new(
        nameof(Development),
        OptimizationMode.Release,
        RuntimeLibraryMode.Release,
        AssertionMode.Enabled,
        AbiMode.Release);

    public static BuildConfigurationProfile Release { get; } = new(
        nameof(Release),
        OptimizationMode.Release,
        RuntimeLibraryMode.Release,
        AssertionMode.Disabled,
        AbiMode.Release);

    public bool IsOptimized => Optimization == OptimizationMode.Release;

    public bool UsesDebugRuntime => RuntimeLibrary == RuntimeLibraryMode.Debug;

    public bool EnablesAssertions => Assertions == AssertionMode.Enabled;

    public bool UsesDebugAbi => Abi == AbiMode.Debug;
}

public static class ConfigurationExtensions
{
    public static BuildConfigurationProfile GetTargetProfile(this Configuration configuration)
    {
        return configuration switch
        {
            Configuration.Debug => BuildConfigurationProfile.Debug,
            Configuration.DebugGame => BuildConfigurationProfile.Debug,
            Configuration.Development => BuildConfigurationProfile.Development,
            Configuration.Shipping => BuildConfigurationProfile.Release,
            _ => throw new ArgumentOutOfRangeException(nameof(configuration), configuration, null)
        };
    }

    public static BuildConfigurationProfile GetEngineProfile(this Configuration configuration)
    {
        return configuration switch
        {
            Configuration.Debug => BuildConfigurationProfile.Debug,
            Configuration.DebugGame => BuildConfigurationProfile.Release,
            Configuration.Development => BuildConfigurationProfile.Release,
            Configuration.Shipping => BuildConfigurationProfile.Release,
            _ => throw new ArgumentOutOfRangeException(nameof(configuration), configuration, null)
        };
    }

    public static BuildConfigurationProfile GetGameProfile(this Configuration configuration)
    {
        return configuration switch
        {
            Configuration.Debug => BuildConfigurationProfile.Debug,
            Configuration.DebugGame => BuildConfigurationProfile.DebugGame,
            Configuration.Development => BuildConfigurationProfile.Development,
            Configuration.Shipping => BuildConfigurationProfile.Release,
            _ => throw new ArgumentOutOfRangeException(nameof(configuration), configuration, null)
        };
    }

    public static bool IsOptimized(this Configuration configuration)
    {
        return configuration.GetTargetProfile().IsOptimized;
    }
}
