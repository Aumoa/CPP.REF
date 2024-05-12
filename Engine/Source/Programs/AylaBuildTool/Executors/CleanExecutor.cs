// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

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
