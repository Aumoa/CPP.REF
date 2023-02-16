// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.ProjectFiles.VisualStudio;
using AE.Projects;

using System.Reflection;

namespace AE.Executors;

public class ProjectFilesExecutor : IExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "projectfile")]
        public string? ProjectFile { get; set; }
    }

    private readonly Arguments GeneratorArgs = new();

    public ProjectFilesExecutor(CommandLineParser Args)
    {
        Args.ApplyTo(GeneratorArgs);
    }

    public async Task<int> RunAsync(CancellationToken CToken = default)
    {
        if (File.Exists(GeneratorArgs.ProjectFile) == false)
        {
            throw new FileNotFoundException($"Project file is not exists. Location: {GeneratorArgs.ProjectFile}");
        }

        string AssemblyLocation = Assembly.GetEntryAssembly()!.Location;
        string EngineLocation = Path.GetFullPath(Path.Combine(AssemblyLocation, "../../../"));
        Workspace Workspace = new(EngineLocation);
        await Workspace.GenerateDirectoriesAsync(CToken);
        await Workspace.GenerateProjectFilesAsync(CToken);

        await VisualStudioGenerator.GenerateSolutionAsync(Workspace, CToken);
        return 0;
    }
}
