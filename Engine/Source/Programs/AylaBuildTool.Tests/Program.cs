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
        new("Configuration.IsOptimized uses target-wide semantics", ConfigurationIsOptimizedUsesTargetWideSemantics),
        new("FolderPolicy uses effective profiles for module paths", FolderPolicyUsesEffectiveProfilesForModulePaths),
        new("FolderPolicy exposes target configuration paths", FolderPolicyExposesTargetConfigurationPaths),
        new("FolderPolicy preserves editor suffix for effective profiles", FolderPolicyPreservesEditorSuffixForEffectiveProfiles),
        new("PCH settings use explicit module header", PchSettingsUseExplicitModuleHeader),
        new("PCH settings require explicit mode and header", PchSettingsRequireExplicitModeAndHeader),
        new("CppCompileCommand classifies PCH commands", CppCompileCommandClassifiesPchCommands)
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

    private static void ConfigurationIsOptimizedUsesTargetWideSemantics()
    {
        AssertEqual(nameof(Configuration.Debug), false, Configuration.Debug.IsOptimized());
        AssertEqual(nameof(Configuration.DebugGame), false, Configuration.DebugGame.IsOptimized());
        AssertEqual(nameof(Configuration.Development), true, Configuration.Development.IsOptimized());
        AssertEqual(nameof(Configuration.Shipping), true, Configuration.Shipping.IsOptimized());
    }

    private static void FolderPolicyUsesEffectiveProfilesForModulePaths()
    {
        var installation = new FakeInstallation();
        var target = Target(Configuration.DebugGame);
        var engineGroup = GroupDescriptor.FromRoot("Engine", SourceGroupKind.Engine);
        var projectGroup = GroupDescriptor.FromRoot("SampleGame", SourceGroupKind.Project);
        var engineProfile = BuildProfileResolver.Resolve(engineGroup, target);
        var projectProfile = BuildProfileResolver.Resolve(projectGroup, target);

        AssertPath(@"Engine\Intermediate\Core\Win64\Release", engineGroup.ModuleIntermediate("Core", target, engineProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"Engine\Binaries\Win64\Release", engineGroup.ModuleOutput(target, engineProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"Engine\Binaries\Win64\Release\Core.out", engineGroup.ModuleOutputFileName(installation, target, engineProfile, "Core", ModuleType.Library, FolderPolicy.PathType.Windows));

        AssertPath(@"SampleGame\Intermediate\GameAssembly\Win64\DebugGame", projectGroup.ModuleIntermediate("GameAssembly", target, projectProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"SampleGame\Binaries\Win64\DebugGame", projectGroup.ModuleOutput(target, projectProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"SampleGame\Binaries\Win64\DebugGame\GameAssembly.out", projectGroup.ModuleOutputFileName(installation, target, projectProfile, "GameAssembly", ModuleType.Game, FolderPolicy.PathType.Windows));
    }

    private static void FolderPolicyExposesTargetConfigurationPaths()
    {
        var installation = new FakeInstallation();
        var target = Target(Configuration.DebugGame);
        var engineGroup = GroupDescriptor.FromRoot("Engine", SourceGroupKind.Engine);

        AssertPath(@"Engine\Intermediate\Core\Win64\DebugGame", engineGroup.TargetIntermediate("Core", target, FolderPolicy.PathType.Windows));
        AssertPath(@"Engine\Binaries\Win64\DebugGame", engineGroup.TargetOutput(target, FolderPolicy.PathType.Windows));
        AssertPath(@"Engine\Binaries\Win64\DebugGame\Core.out", engineGroup.TargetOutputFileName(installation, target, "Core", ModuleType.Library, FolderPolicy.PathType.Windows));
    }

    private static void FolderPolicyPreservesEditorSuffixForEffectiveProfiles()
    {
        var target = Target(Configuration.DebugGame, editor: true);
        var engineGroup = GroupDescriptor.FromRoot("Engine", SourceGroupKind.Engine);
        var projectGroup = GroupDescriptor.FromRoot("SampleGame", SourceGroupKind.Project);
        var engineProfile = BuildProfileResolver.Resolve(engineGroup, target);
        var projectProfile = BuildProfileResolver.Resolve(projectGroup, target);

        AssertPath(@"Engine\Intermediate\Core\Win64\Release-Editor", engineGroup.ModuleIntermediate("Core", target, engineProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"Engine\Binaries\Win64\Release-Editor", engineGroup.ModuleOutput(target, engineProfile, FolderPolicy.PathType.Windows));

        AssertPath(@"SampleGame\Intermediate\GameAssembly\Win64\DebugGame-Editor", projectGroup.ModuleIntermediate("GameAssembly", target, projectProfile, FolderPolicy.PathType.Windows));
        AssertPath(@"SampleGame\Binaries\Win64\DebugGame-Editor", projectGroup.ModuleOutput(target, projectProfile, FolderPolicy.PathType.Windows));
    }

    private static void PchSettingsUseExplicitModuleHeader()
    {
        var environment = CompileEnvironmentFor<PchEnabledRules>("PchEnabledModule");
        var pchSettings = environment.PchSettings ?? throw new InvalidOperationException("Expected PCH settings.");

        AssertEqual("PCH header", "CoreMinimal.h", pchSettings.HeaderIncludeName);
        AssertEqual("PCH output name", "PchEnabledModule.pch", pchSettings.OutputName);
        AssertPath(@"Engine\Intermediate\PchEnabledModule\Win64\Release\PchEnabledModule.pch.cpp", pchSettings.SourceFilePath);
        AssertPath(@"Engine\Intermediate\PchEnabledModule\Win64\Release\PchEnabledModule.pch", pchSettings.PchFilePath);
        AssertPath(@"Engine\Intermediate\PchEnabledModule\Win64\Release\PchEnabledModule.pch.pdb", pchSettings.PdbFilePath);
        AssertPath(@"Engine\Intermediate\PchEnabledModule\Win64\Release\PchEnabledModule.pch.cache", pchSettings.CacheFilePath);
    }

    private static void PchSettingsRequireExplicitModeAndHeader()
    {
        AssertNull("Default PCH usage should not create settings", CompileEnvironmentFor<PchDefaultRules>("PchDefaultModule").PchSettings);
        AssertNull("Explicit PCH usage without a header should not create settings", CompileEnvironmentFor<PchNoHeaderRules>("PchNoHeaderModule").PchSettings);
    }

    private static void CppCompileCommandClassifiesPchCommands()
    {
        var environment = CompileEnvironmentFor<PchEnabledRules>("PchCommandModule");
        var pchSettings = environment.PchSettings ?? throw new InvalidOperationException("Expected PCH settings.");

        var pchCommand = CppCompileCommand.CreatePch(environment);
        AssertEqual("PCH create command", CppPchCommandKind.Create, pchCommand.PchCommandKind);
        AssertEqual("PCH create command creates PCH", true, pchCommand.CreatesPch);
        AssertEqual("PCH create command uses PCH", false, pchCommand.UsesPch);
        AssertEqual("PCH create output", pchSettings.OutputName, pchCommand.OutputName);

        var sourceCommand = new CppCompileCommand(environment, SourceCode(environment, @"Private\Main.cpp", SourceCodeType.SourceCode));
        AssertEqual("Source command", CppPchCommandKind.Use, sourceCommand.PchCommandKind);
        AssertEqual("Source command creates PCH", false, sourceCommand.CreatesPch);
        AssertEqual("Source command uses PCH", true, sourceCommand.UsesPch);
        AssertSequence(sourceCommand.CacheDependencyFilePaths, pchSettings.CacheFilePath);

        var headerCommand = new CppCompileCommand(environment, SourceCode(environment, @"Public\Main.h", SourceCodeType.Header));
        AssertEqual("Header command", CppPchCommandKind.None, headerCommand.PchCommandKind);
        AssertEqual("Header command creates PCH", false, headerCommand.CreatesPch);
        AssertEqual("Header command uses PCH", false, headerCommand.UsesPch);
        AssertSequence(headerCommand.CacheDependencyFilePaths);
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

    private static CppCompileEnvironment CompileEnvironmentFor<TRules>(string moduleName)
        where TRules : ModuleRules
    {
        var target = Target(Configuration.Shipping);
        var engineGroup = GroupDescriptor.FromRoot("Engine", SourceGroupKind.Engine);
        var sourceDirectory = Path.Combine("Engine", "Source", "Runtime", moduleName);
        var project = new ModuleProject(
            moduleName,
            engineGroup,
            sourceDirectory,
            typeof(TRules),
            Path.Combine(sourceDirectory, moduleName + ".Module.cs"),
            ModuleProject.ModuleDeclaration.New());
        var solution = new Solution(null, [project], engineGroup, engineGroup);
        var rules = ModuleRules.New(typeof(TRules), target);
        rules.ThrowErrors();
        var resolver = new ModuleRulesResolver(target, solution, project, rules);

        return new CppCompileEnvironment(resolver, target, engineGroup);
    }

    private static SourceCodeDescriptor SourceCode(CppCompileEnvironment environment, string relativePath, SourceCodeType type)
    {
        return new SourceCodeDescriptor(
            environment.Descriptor,
            environment.Resolver.Name,
            Path.Combine(environment.Resolver.Project.SourceDirectory, relativePath),
            relativePath,
            type);
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

    private static void AssertNull<T>(string context, T? actual)
    {
        if (actual != null)
        {
            throw new InvalidOperationException($"{context}: expected null, but got '{actual}'.");
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

    private sealed class PchEnabledRules : ModuleRules
    {
        public PchEnabledRules()
        {
            PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
            PrivatePchHeaderFile = " CoreMinimal.h ";
        }
    }

    private sealed class PchNoHeaderRules : ModuleRules
    {
        public PchNoHeaderRules()
        {
            PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        }
    }

    private sealed class PchDefaultRules : ModuleRules
    {
        public PchDefaultRules()
        {
            PrivatePchHeaderFile = "CoreMinimal.h";
        }
    }

    private readonly record struct TestCase(string Name, Action Run);
}
