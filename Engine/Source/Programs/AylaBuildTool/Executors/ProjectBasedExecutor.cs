// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.Projects;

using System.Reflection;

namespace AE.Executors;

public abstract class ProjectBasedExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "engine")]
        public string? EngineDirectory { get; set; }
    }

    private readonly Arguments Args = new();

    protected ProjectBasedExecutor(CommandLineParser Args)
    {
        Args.ApplyTo(this.Args);
    }

    protected Workspace GenerateEngineWorkspace()
    {
        if (Args.EngineDirectory != null)
        {
            string BuildToolPath = Path.Combine(Args.EngineDirectory, "Binaries/DotNET/AylaBuildTool.dll");
            if (File.Exists(BuildToolPath) == false)
            {
                throw new InvalidOperationException(CoreStrings.Errors.InvalidEngineDirectory);
            }

            return new Workspace(Args.EngineDirectory, BuildToolPath);
        }

        var CurrentAssembly = Assembly.GetEntryAssembly();
        if (CurrentAssembly == null)
        {
            throw new InvalidOperationException(CoreStrings.Errors.AssemblyLocationNotFound);
        }

        string AssemblyLocation = CurrentAssembly.Location;
        string EngineLocation = Path.GetFullPath(Path.Combine(AssemblyLocation, "../../../"));

        return new Workspace(EngineLocation, AssemblyLocation);
    }
}
