// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.CLI;
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
        string NinjaExecutable = Path.Combine(Workspace.EngineDirectory.BuildDirectory, "Ninja", PlatformId, "bin", "ninja");
        string CCompiler = "";
        string CXXCompiler = "";

        if (TargetInfo.BuildConfiguration.Platform.Group == PlatformGroup.Windows)
        {
            CMakeExecutable = Path.ChangeExtension(CMakeExecutable, ".exe");
            NinjaExecutable = Path.ChangeExtension(NinjaExecutable, ".exe");
            CCompiler = "cl.exe";
            CXXCompiler = "cl.exe";
        }
        else if (TargetInfo.BuildConfiguration.Platform.Group == PlatformGroup.Linux)
        {
            CCompiler = "gcc";
            CXXCompiler = "g++";
        }

        Console.WriteLine("Start building with CMake...");
        foreach (var CXXProject in Solution.CXXProjects)
        {
            string ProjectFilesDir = Directory.GetParent(CXXProject.MakefilePath)!.FullName;
            string ProjectName = CXXProject.Name;
            string IntermediatePath = Path.GetFullPath(Path.Combine(ProjectFilesDir, "../../.."));
            IntermediatePath = Path.Combine(IntermediatePath, "Shipping", ProjectName);
            if (BuildArgs.bClean && Directory.Exists(IntermediatePath))
            {
                Directory.Delete(IntermediatePath, true);
            }
            Directory.CreateDirectory(IntermediatePath);

            ProcessStartInfo CmdStart = new(CMakeExecutable)
            {
                Arguments = $"-G Ninja -DCMAKE_MAKE_PROGRAM=\"{NinjaExecutable}\" -DCMAKE_C_COMPILER=\"{CCompiler}\" -DCMAKE_CXX_COMPILER=\"{CXXCompiler}\" \"{CXXProject.MakefilePath}\"",
                WorkingDirectory = IntermediatePath,
#if !DEBUG
                RedirectStandardOutput = true,
#endif
            };

            Process? CMakeApp = Process.Start(CmdStart);
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

            CmdStart = new(CMakeExecutable)
            {
                Arguments = $"--build .",
                WorkingDirectory = IntermediatePath
            };

            CMakeApp = Process.Start(CmdStart);
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
