// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.ProjectFiles.CMake;
using AE.Projects;

using System.Diagnostics;

namespace AE.Executors;

public class BuildExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "projectfile")]
        public string? ProjectFile { get; set; }
    }

    private readonly Arguments BuildArgs = new();

    public BuildExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(BuildArgs);
    }

    public async Task<int> RunAsync(CancellationToken CToken = default)
    {
        await Task.Yield();
        Workspace Workspace = GenerateEngineWorkspace();
        await Workspace.GenerateDirectoriesAsync(CToken);
        await Workspace.GenerateProjectFilesAsync(CToken);

        Console.WriteLine("Generate CMake projects.");
        CMakeSolution Solution = await CMakeGenerator.GenerateSolutionAsync(Workspace, null, CToken);

        string CMakeExecutable = Path.Combine(Workspace.EngineDirectory.BuildDirectory, "CMake", "Win64", "bin", "cmake.exe");
        string NinjaExecutable = Path.Combine(Workspace.EngineDirectory.BuildDirectory, "Ninja", "Win64", "bin", "ninja.exe");

        Console.WriteLine("Start building with CMake...");
        foreach (var CXXProject in Solution.CXXProjects)
        {
            string ProjectName = CXXProject.Name;
            string IntermediatePath = Path.Combine(CXXProject.MakefilePath, "../../..");
            IntermediatePath = Path.Combine(IntermediatePath, "Shipping", ProjectName);
            Directory.CreateDirectory(IntermediatePath);

            ProcessStartInfo CmdStart = new(CMakeExecutable)
            {
                Arguments = $"-G Ninja -DCMAKE_MAKE_PROGRAM=\"{NinjaExecutable}\" -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe \"{CXXProject.MakefilePath}\"",
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
