namespace AylaEngine.Tests;

internal static class Program
{
    private static readonly TestCase[] s_Tests =
    [
        new("Unix dependencies on target line", UnixDependenciesOnTargetLine),
        new("Unix dependencies on continuation lines", UnixDependenciesOnContinuationLines),
        new("Unix escaped paths and empty phony rules", UnixEscapedPathsAndEmptyPhonyRules),
        new("BuildProfileResolver maps engine configurations", BuildProfileResolverMapsEngineConfigurations),
        new("BuildProfileResolver maps project configurations", BuildProfileResolverMapsProjectConfigurations),
        new("BuildConfigurationProfile exposes compiler policy flags", BuildConfigurationProfileExposesCompilerPolicyFlags),
        new("FolderPolicy uses effective profiles for module paths", FolderPolicyUsesEffectiveProfilesForModulePaths),
        new("FolderPolicy preserves editor suffix for effective profiles", FolderPolicyPreservesEditorSuffixForEffectiveProfiles)
    ];

    private static int Main()
    {
        List<string> failures = [];

        foreach (var test in s_Tests)
        {
            Run(test.Name, failures, test.Run);
        }

        if (failures.Count == 0)
        {
            Console.WriteLine("AylaBuildTool.Tests passed.");
            return 0;
        }

        foreach (var failure in failures)
        {
            Console.Error.WriteLine(failure);
        }

        return 1;
    }

    private static void UnixDependenciesOnTargetLine()
    {
        var actual = UnixInstallation.ParseMakeRuleDependencies(
            "obj/main.o: src/main.cpp include/CoreMinimal.h\r\n");
        AssertSequence(actual, "src/main.cpp", "include/CoreMinimal.h");
    }

    private static void UnixDependenciesOnContinuationLines()
    {
        var actual = UnixInstallation.ParseMakeRuleDependencies(
            "obj/main.o: src/main.cpp \\\r\n include/CoreMinimal.h \\\r\n include/PCH.h\r\n");
        AssertSequence(actual, "src/main.cpp", "include/CoreMinimal.h", "include/PCH.h");
    }

    private static void UnixEscapedPathsAndEmptyPhonyRules()
    {
        var actual = UnixInstallation.ParseMakeRuleDependencies(
            "C\\:/obj/main.o: src/main.cpp include/path\\ with\\ space.h C\\:/include/PCH.h\r\n" +
            "include/path\\ with\\ space.h:\r\n");
        AssertSequence(actual, "src/main.cpp", "include/path with space.h", "C:/include/PCH.h");
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

    private static void FolderPolicyUsesEffectiveProfilesForModulePaths()
    {
        var installation = new FakeInstallation();
        var target = Target(Configuration.DebugGame);
        var engineGroup = GroupDescriptor.FromRoot("Engine", SourceGroupKind.Engine);
        var projectGroup = GroupDescriptor.FromRoot("SampleGame", SourceGroupKind.Project);
        var engineProfile = BuildProfileResolver.Resolve(engineGroup, target);
        var projectProfile = BuildProfileResolver.Resolve(projectGroup, target);

        AssertPath(@"Engine\Intermediate\Core\Win64\Release", engineGroup.Intermediate("Core", target, engineProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"Engine\Binaries\Win64\Release", engineGroup.Output(target, engineProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"Engine\Binaries\Win64\Release\Core.out", engineGroup.OutputFileName(installation, target, engineProfile, "Core", ModuleType.Library, FolderPolicy.PathType.Windows));

        AssertPath(@"SampleGame\Intermediate\GameAssembly\Win64\DebugGame", projectGroup.Intermediate("GameAssembly", target, projectProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"SampleGame\Binaries\Win64\DebugGame", projectGroup.Output(target, projectProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"SampleGame\Binaries\Win64\DebugGame\GameAssembly.out", projectGroup.OutputFileName(installation, target, projectProfile, "GameAssembly", ModuleType.Game, FolderPolicy.PathType.Windows));
    }

    private static void FolderPolicyPreservesEditorSuffixForEffectiveProfiles()
    {
        var target = Target(Configuration.DebugGame, editor: true);
        var engineGroup = GroupDescriptor.FromRoot("Engine", SourceGroupKind.Engine);
        var projectGroup = GroupDescriptor.FromRoot("SampleGame", SourceGroupKind.Project);
        var engineProfile = BuildProfileResolver.Resolve(engineGroup, target);
        var projectProfile = BuildProfileResolver.Resolve(projectGroup, target);

        AssertPath(@"Engine\Intermediate\Core\Win64\Release-Editor", engineGroup.Intermediate("Core", target, engineProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"Engine\Binaries\Win64\Release-Editor", engineGroup.Output(target, engineProfile, FolderPolicy.PathType.Windows));

        AssertPath(@"SampleGame\Intermediate\GameAssembly\Win64\DebugGame-Editor", projectGroup.Intermediate("GameAssembly", target, projectProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"SampleGame\Binaries\Win64\DebugGame-Editor", projectGroup.Output(target, projectProfile, FolderPolicy.PathType.Windows));
    }

    private static void Run(string name, List<string> failures, Action test)
    {
        try
        {
            test();
            Console.WriteLine($"[PASS] {name}");
        }
        catch (Exception exception)
        {
            failures.Add($"[FAIL] {name}: {exception.Message}");
        }
    }

    private static void AssertSequence(string[] actual, params string[] expected)
    {
        if (actual.SequenceEqual(expected))
        {
            return;
        }

        throw new InvalidOperationException(
            $"Expected [{string.Join(", ", expected)}], actual [{string.Join(", ", actual)}].");
    }

    private static TargetInfo Target(Configuration configuration, bool editor = false)
    {
        return new TargetInfo
        {
            Platform = PlatformInfo.Win64,
            Config = configuration,
            Editor = editor
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

    private static void AssertPath(string expected, string actual)
    {
        if (actual != expected)
        {
            throw new InvalidOperationException($"Expected path '{expected}', but got '{actual}'.");
        }
    }

    private sealed class FakeInstallation : Installation
    {
        public override ValueTask<string> GetCompilerPath(TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            throw new NotSupportedException();
        }

        public override ValueTask<string> GetIntelliSenseMode(TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            throw new NotSupportedException();
        }

        public override ValueTask<CppCompiler> SpawnCompilerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            throw new NotSupportedException();
        }

        public override ValueTask<Linker> SpawnLinkerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            throw new NotSupportedException();
        }

        public override ValueTask<string[]> ParseDependenciesAsync(string depsFileName, CancellationToken cancellationToken)
        {
            throw new NotSupportedException();
        }

        public override string OutputFileName(string projectName, ModuleType moduleType)
        {
            return projectName + ".out";
        }
    }

    private readonly record struct TestCase(string Name, Action Run);
}
