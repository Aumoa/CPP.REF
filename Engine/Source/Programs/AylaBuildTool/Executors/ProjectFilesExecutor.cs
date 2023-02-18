// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.ProjectFiles.VisualStudio;
using AE.Projects;

namespace AE.Executors;

public class ProjectFilesExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "projectfile")]
        public string? ProjectFile { get; set; }
    }

    private readonly Arguments GeneratorArgs = new();

    public ProjectFilesExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(GeneratorArgs);
    }

    public async Task<int> RunAsync(CancellationToken CToken = default)
    {
        Workspace Workspace = GenerateEngineWorkspace();
        await Workspace.GenerateDirectoriesAsync(CToken);
        await Workspace.GenerateProjectFilesAsync(CToken);

        await VisualStudioGenerator.GenerateSolutionAsync(Workspace, null, CToken);
        return 0;
    }
}
