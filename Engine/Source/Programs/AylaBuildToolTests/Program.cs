namespace AylaEngine.Tests;

internal static class Program
{
    private static readonly TestCase[] s_Tests =
    [
        new("BuildProfileResolver maps engine configurations", BuildProfileResolverMapsEngineConfigurations),
        new("BuildProfileResolver maps project configurations", BuildProfileResolverMapsProjectConfigurations),
        new("BuildConfigurationProfile exposes compiler policy flags", BuildConfigurationProfileExposesCompilerPolicyFlags)
    ];

    private static int Main()
    {
        try
        {
            foreach (var test in s_Tests)
            {
                test.Run();
                Console.WriteLine("[PASS] " + test.Name);
            }

            Console.WriteLine("All AylaBuildTool policy tests passed.");
            return 0;
        }
        catch (Exception e)
        {
            Console.Error.WriteLine(e.Message);
            return 1;
        }
    }

    private static void BuildProfileResolverMapsEngineConfigurations()
    {
        var group = GroupDescriptor.FromRoot("Engine", SourceGroupKind.Engine);

        AssertProfile(BuildProfileResolver.Resolve(group, Target(Configuration.Debug)), BuildConfigurationProfile.Debug);
        AssertProfile(BuildProfileResolver.Resolve(group, Target(Configuration.DebugGame)), BuildConfigurationProfile.Release);
        AssertProfile(BuildProfileResolver.Resolve(group, Target(Configuration.Development)), BuildConfigurationProfile.Release);
        AssertProfile(BuildProfileResolver.Resolve(group, Target(Configuration.Shipping)), BuildConfigurationProfile.Release);
    }

    private static void BuildProfileResolverMapsProjectConfigurations()
    {
        var group = GroupDescriptor.FromRoot("SampleGame", SourceGroupKind.Project);

        AssertProfile(BuildProfileResolver.Resolve(group, Target(Configuration.Debug)), BuildConfigurationProfile.Debug);
        AssertProfile(BuildProfileResolver.Resolve(group, Target(Configuration.DebugGame)), BuildConfigurationProfile.DebugGame);
        AssertProfile(BuildProfileResolver.Resolve(group, Target(Configuration.Development)), BuildConfigurationProfile.Development);
        AssertProfile(BuildProfileResolver.Resolve(group, Target(Configuration.Shipping)), BuildConfigurationProfile.Release);
    }

    private static void BuildConfigurationProfileExposesCompilerPolicyFlags()
    {
        AssertCompilerPolicy(
            BuildConfigurationProfile.Debug,
            OptimizationMode.Debug,
            RuntimeLibraryMode.Debug,
            AssertionMode.Enabled,
            AbiMode.Debug,
            isOptimized: false,
            usesDebugRuntime: true,
            enablesAssertions: true,
            usesDebugAbi: true);

        AssertCompilerPolicy(
            BuildConfigurationProfile.DebugGame,
            OptimizationMode.Debug,
            RuntimeLibraryMode.Release,
            AssertionMode.Enabled,
            AbiMode.Release,
            isOptimized: false,
            usesDebugRuntime: false,
            enablesAssertions: true,
            usesDebugAbi: false);

        AssertCompilerPolicy(
            BuildConfigurationProfile.Development,
            OptimizationMode.Release,
            RuntimeLibraryMode.Release,
            AssertionMode.Enabled,
            AbiMode.Release,
            isOptimized: true,
            usesDebugRuntime: false,
            enablesAssertions: true,
            usesDebugAbi: false);

        AssertCompilerPolicy(
            BuildConfigurationProfile.Release,
            OptimizationMode.Release,
            RuntimeLibraryMode.Release,
            AssertionMode.Disabled,
            AbiMode.Release,
            isOptimized: true,
            usesDebugRuntime: false,
            enablesAssertions: false,
            usesDebugAbi: false);
    }

    private static TargetInfo Target(Configuration configuration)
    {
        return new TargetInfo
        {
            Platform = PlatformInfo.Win64,
            Config = configuration,
            Editor = false
        };
    }

    private static void AssertProfile(BuildConfigurationProfile actual, BuildConfigurationProfile expected)
    {
        if (actual != expected)
        {
            throw new InvalidOperationException($"Expected profile '{expected.Name}', but got '{actual.Name}'.");
        }
    }

    private static void AssertCompilerPolicy(
        BuildConfigurationProfile profile,
        OptimizationMode optimization,
        RuntimeLibraryMode runtimeLibrary,
        AssertionMode assertions,
        AbiMode abi,
        bool isOptimized,
        bool usesDebugRuntime,
        bool enablesAssertions,
        bool usesDebugAbi)
    {
        AssertEqual(profile.Name, optimization, profile.Optimization);
        AssertEqual(profile.Name, runtimeLibrary, profile.RuntimeLibrary);
        AssertEqual(profile.Name, assertions, profile.Assertions);
        AssertEqual(profile.Name, abi, profile.Abi);
        AssertEqual(profile.Name, isOptimized, profile.IsOptimized);
        AssertEqual(profile.Name, usesDebugRuntime, profile.UsesDebugRuntime);
        AssertEqual(profile.Name, enablesAssertions, profile.EnablesAssertions);
        AssertEqual(profile.Name, usesDebugAbi, profile.UsesDebugAbi);
    }

    private static void AssertEqual<T>(string profileName, T expected, T actual)
    {
        if (EqualityComparer<T>.Default.Equals(actual, expected) == false)
        {
            throw new InvalidOperationException($"Profile '{profileName}' expected '{expected}', but got '{actual}'.");
        }
    }

    private readonly record struct TestCase(string Name, Action Run);
}
