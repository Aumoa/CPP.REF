// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.ProjectFiles.VisualStudio;
using AE.Projects;

namespace AE.Executors;

public class ProjectFilesExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
    }

    private readonly Arguments GeneratorArgs = new();

    public ProjectFilesExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(GeneratorArgs);
    }

    public async Task<int> RunAsync(CancellationToken cancellationToken = default)
    {
        Workspace Workspace = await ConfigureWorkspaceAsync(cancellationToken);
        VisualStudioSolution Solution = new(Workspace, ProjectFile);
        await Solution.ConfigureRulesAsync(cancellationToken);
        await Solution.GenerateProjectFilesAsync(cancellationToken);
        return 0;
    }
}
