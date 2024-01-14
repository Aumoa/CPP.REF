// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.Projects;

namespace AE.Executors;

public class CleanExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
    }

    private readonly Arguments buildArgs = new();

    public CleanExecutor(CommandLineParser args) : base(args)
    {
        args.ApplyTo(buildArgs);
    }

    public Task<int> RunAsync(CancellationToken cancellationToken = default)
    {
        ConfigureWorkspace();
        Workspace.Cleanup();
        return Task.FromResult(0);
    }
}
