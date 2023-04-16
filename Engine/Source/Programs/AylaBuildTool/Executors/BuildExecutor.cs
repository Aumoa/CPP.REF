// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.CLI;
using AE.Platform;
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

        [CommandLineApply(CategoryName = "Target", IsRequired = true)]
        public string Target { get; set; } = null!;

        [CommandLineApply(CategoryName = "Editor")]
        public bool bEditor { get; set; }
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
            //Name = BuildArgs.Target,
            BuildConfiguration = new BuildConfiguration()
            {
                Configuration = Enum.Parse<Configuration>(BuildArgs.Config!, true),
                Platform = BuildHostPlatform.Current.Platform
            },
            bEditor = BuildArgs.bEditor
        };

        await Task.Yield();
        Workspace Workspace = GenerateEngineWorkspace();
        await Workspace.GenerateDirectoriesAsync(CToken);
        await Workspace.GenerateProjectFilesAsync(TargetInfo, CToken);

        Console.WriteLine("Generate CMake projects.");
        CMakeSolution Solution = await CMakeGenerator.GenerateSolutionAsync(Workspace, null, CToken);

        string PlatformId = TargetInfo.BuildConfiguration.Platform.ToString()!;

        //string CMakeExecutable = Path.Combine(Workspace.EngineDirectory.BuildDirectory, "CMake", PlatformId, "bin", "cmake");
        string CMakeExecutable = "";
        List<string> PathEnvironments = new();
        List<string> LibraryEnvironments = new();
        List<string> IncludeEnvironments = new();
        Dictionary<string, string> CMakeDefinitions = new();

        // Add ninja executable path to environments.
        //PathEnvironments.Add(Path.Combine(Workspace.EngineDirectory.BuildDirectory, "Ninja", PlatformId, "bin"));
        PathEnvironments.Add(Environment.SystemDirectory);

        ToolChainInstallation? ToolChain = null;
        const Architecture CurrentArchitecture = Architecture.x64;

        if (TargetInfo.BuildConfiguration.Platform.Group == PlatformGroup.Windows)
        {
            // Windows executable file must be ends with .exe.
            CMakeExecutable = Path.ChangeExtension(CMakeExecutable, ".exe");

            ToolChain = VisualStudioInstallation.FindVisualStudioInstallations(Compiler.VisualStudio2022).First();

            CMakeDefinitions.Add("CMAKE_CXX_FLAGS", "/EHsc /MP /std:c++20");
            CMakeDefinitions.Add("CMAKE_C_COMPILER", "cl");
            CMakeDefinitions.Add("CMAKE_CXX_COMPILER", "cl");
            CMakeDefinitions.Add("CMAKE_SHARED_LIBRARY_EXPORT", "__declspec(dllexport)");
            CMakeDefinitions.Add("CMAKE_SHARED_LIBRARY_IMPORT", "__declspec(dllimport)");
        }
        else if (TargetInfo.BuildConfiguration.Platform.Group == PlatformGroup.Linux)
        {
            CMakeDefinitions.Add("CMAKE_CXX_FLAGS", "std=c++20");
            CMakeDefinitions.Add("CMAKE_C_COMPILER", "gcc");
            CMakeDefinitions.Add("CMAKE_CXX_COMPILER", "g++");
            CMakeDefinitions.Add("CMAKE_SHARED_LIBRARY_EXPORT", "__attribute__((visibility(\\\"default\\\")))");
            CMakeDefinitions.Add("CMAKE_SHARED_LIBRARY_IMPORT", "");
        }
        else
        {
            Console.Error.WriteLine("Not supported platform.");
            return 1;
        }

        if (ToolChain != null)
        {
            PathEnvironments.AddRange(ToolChain.GetRequiredExecutablePaths(CurrentArchitecture));
            LibraryEnvironments.AddRange(ToolChain.GetRequiredLibraryPaths(CurrentArchitecture));
            IncludeEnvironments.AddRange(ToolChain.GetRequiredIncludePaths(CurrentArchitecture));
        }

        Console.WriteLine("Start building with CMake...");
        foreach (var CXXProject in Solution.CXXProjects)
        {
            if (CXXProject.bEmptyProject)
            {
                // Skip project.
                continue;
            }

            string ProjectFilesDir = Directory.GetParent(CXXProject.MakefilePath)!.FullName;
            string ProjectName = CXXProject.Name;
            //string IntermediatePath = Workspace.TargetDirectory.IntermediateDirectory;
            string IntermediatePath = "";
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
            CMakeStart.Environment["LIB"] = string.Join(';', LibraryEnvironments);

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

            CMakeStart.Environment["PATH"] = string.Join(';', PathEnvironments);
            CMakeStart.Environment["LIB"] = string.Join(';', LibraryEnvironments);
            CMakeStart.Environment["INCLUDE"] = string.Join(';', IncludeEnvironments);

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
