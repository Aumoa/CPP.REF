// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.Projects;

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
        Console.WriteLine("Generate CMake projects.");
        return 0;
    }
}
