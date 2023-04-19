// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.Projects;

namespace AE.Executors;

public class CleanExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
    }

    private readonly Arguments BuildArgs = new();

    public CleanExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(BuildArgs);
    }

    public Task<int> RunAsync(CancellationToken SToken = default)
    {
        foreach (var Dir in GetProjectDirectories())
        {
            Dir.Cleanup();
        }
        return Task.FromResult(0);
    }
}
