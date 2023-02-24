// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.CLI;
using AE.Platform.Windows;
using AE.ProjectFiles.CMake;
using AE.Projects;
using AE.Rules;
using AE.System;

using System.Diagnostics;

namespace AE.Executors;

public class BuildExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "projectfile")]
        public string? ProjectFile { get; set; }

        [CommandLineApply(CategoryName = "Config")]
        public string? Config { get; set; }

        [CommandLineApply(CategoryName = "Clean")]
        public bool bClean { get; set; }
    }

    private readonly Arguments BuildArgs = new();

    public BuildExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(BuildArgs);
        BuildArgs.Config ??= "Development";
    }

    public virtual async Task<int> RunAsync(CancellationToken CToken = default)
    {
        var TargetInfo = new TargetInfo()
        {
            BuildConfiguration = new BuildConfiguration()
            {
                Configuration = Enum.Parse<Configuration>(BuildArgs.Config!, true),
                Platform = BuildHostPlatform.Current.Platform
            }
        };

        await Task.Yield();
        Workspace Workspace = GenerateEngineWorkspace();
        await Workspace.GenerateDirectoriesAsync(CToken);
        await Workspace.GenerateProjectFilesAsync(TargetInfo, CToken);

        Console.WriteLine("Generate CMake projects.");
        CMakeSolution Solution = await CMakeGenerator.GenerateSolutionAsync(Workspace, null, CToken);

        string PlatformId = TargetInfo.BuildConfiguration.Platform.ToString();

        string CMakeExecutable = Path.Combine(Workspace.EngineDirectory.BuildDirectory, "CMake", PlatformId, "bin", "cmake");
        List<string> PathEnvironments = new();
        Dictionary<string, string> CMakeDefinitions = new();

        // Add current path environments.
        string CurrentPaths = Environment.GetEnvironmentVariable("PATH") ?? "";
        foreach (var Path in CurrentPaths.Split(';', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries))
        {
            PathEnvironments.Add(Path);
        }

        // Add ninja executable path to environments.
        PathEnvironments.Add(Path.Combine(Workspace.EngineDirectory.BuildDirectory, "Ninja", PlatformId, "bin"));

        if (TargetInfo.BuildConfiguration.Platform.Group == PlatformGroup.Windows)
        {
            CMakeExecutable = Path.ChangeExtension(CMakeExecutable, ".exe");

            var Installations = VisualStudioInstallation.FindVisualStudioInstallations(Compiler.VisualStudio2022);
            string CompilerPath = Installations.First().FindCCompilerPath(Platform.Architecture.x64);

            PathEnvironments.Add(Path.GetDirectoryName(CompilerPath)!);
            PathEnvironments.Add("C:/Program Files (x86)/Windows Kits/10\\bin\\10.0.19041.0\\x64");

            CMakeDefinitions.Add("CMAKE_CXX_FLAGS", "/EHsc /MP");

            CMakeDefinitions.Add("CMAKE_SHARED_LIBRARY_EXPORT", "__declspec(dllexport)");
            CMakeDefinitions.Add("CMAKE_SHARED_LIBRARY_IMPORT", "__declspec(dllimport)");
        }
        else if (TargetInfo.BuildConfiguration.Platform.Group == PlatformGroup.Linux)
        {
            CMakeDefinitions.Add("CMAKE_CXX_FLAGS", "");
            CMakeDefinitions.Add("CMAKE_C_COMPILER", "gcc");
            CMakeDefinitions.Add("CMAKE_CXX_COMPILER", "g++");

            CMakeDefinitions.Add("CMAKE_SHARED_LIBRARY_EXPORT", "__attribute__((visibility(\\\"default\\\")))");
            CMakeDefinitions.Add("CMAKE_SHARED_LIBRARY_IMPORT", "");
        }

        Console.WriteLine("Start building with CMake...");
        foreach (var CXXProject in Solution.CXXProjects)
        {
            string ProjectFilesDir = Directory.GetParent(CXXProject.MakefilePath)!.FullName;
            string ProjectName = CXXProject.Name;
            string IntermediatePath = Workspace.TargetDirectory.IntermediateDirectory;
            IntermediatePath = Path.Combine(IntermediatePath, "Build", "Shipping", ProjectName);
            if (BuildArgs.bClean && Directory.Exists(IntermediatePath))
            {
                Directory.Delete(IntermediatePath, true);
            }
            Directory.CreateDirectory(IntermediatePath);

            ProcessStartInfo CMakeStart = new(CMakeExecutable)
            {
                Arguments = $"-GNinja {string.Join(' ', CMakeDefinitions.Select(p => $"-D{p.Key}=\"{p.Value}\""))} \"{CXXProject.MakefilePath}\"",
                WorkingDirectory = IntermediatePath,
#if !DEBUG
                RedirectStandardOutput = true,
#endif
            };

            CMakeStart.Environment["PATH"] = string.Join(';', PathEnvironments);

            Process? CMakeApp = Process.Start(CMakeStart);
            if (CMakeApp == null)
            {
                Console.Error.WriteLine(CoreStrings.Errors.StartCMakeError);
                return -1;
            }

            await CMakeApp.WaitForExitAsync(CToken);
            if (CMakeApp.ExitCode != 0)
            {
                Console.Error.WriteLine(CoreStrings.Errors.CMakeReturnCodeError, CMakeApp.ExitCode);
                return CMakeApp.ExitCode;
            }

            CMakeStart = new(CMakeExecutable)
            {
                Arguments = $"--build .",
                WorkingDirectory = IntermediatePath
            };

            CMakeApp = Process.Start(CMakeStart);
            if (CMakeApp == null)
            {
                Console.Error.WriteLine(CoreStrings.Errors.StartCMakeError);
                return -1;
            }

            await CMakeApp.WaitForExitAsync(CToken);
            if (CMakeApp.ExitCode != 0)
            {
                Console.Error.WriteLine(CoreStrings.Errors.CMakeReturnCodeError, CMakeApp.ExitCode);
                return CMakeApp.ExitCode;
            }
        }

        return 0;
    }
}
