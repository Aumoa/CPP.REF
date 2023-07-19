// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.BuildSettings;
using AE.CLI;
using AE.Exceptions;
using AE.Misc;
using AE.Platform;
using AE.Platform.Windows;
using AE.Projects;
using AE.Rules;
using AE.SourceTree;
using AE.System;

namespace AE.Executors;

public class BuildExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "Target", IsRequired = true)]
        public string Target { get; set; } = null!;

        [CommandLineApply(CategoryName = "Clean")]
        public bool bClean { get; set; }
    }

    private readonly Arguments BuildArgs = new();

    public BuildExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(BuildArgs);
    }

    public async Task<int> RunAsync(CancellationToken SToken = default)
    {
        if (BuildArgs.bClean == true)
        {
            foreach (var Dir in GetProjectDirectories())
            {
                Dir.Cleanup();
            }
        }

        Workspace Workspace = await ConfigureWorkspaceAsync(SToken);

        var HeaderToolCompiler = await AylaProjectCompiler.CreateCompilerAsync(Workspace, "AylaHeaderTool", new TargetInfo
        {
            BuildConfiguration = new()
            {
                Configuration = Configuration.Shipping,
                Platform = TargetPlatform.Win64
            }
        }, SToken);

        int ReturnCode = await HeaderToolCompiler.CompileAsync(SToken);
        if (ReturnCode != 0)
        {
            return ReturnCode;
        }

        var ProjectCompiler = await AylaProjectCompiler.CreateCompilerAsync(Workspace, BuildArgs.Target, new TargetInfo
        {
            BuildConfiguration = new()
            {
                Configuration = Configuration.Shipping,
                Platform = TargetPlatform.Win64
            }
        });

        return await ProjectCompiler.CompileAsync(SToken);
    }
}
