// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.CLI;
using AE.Exceptions;
using AE.ProjectFiles.VisualStudio;
using AE.Projects;
using AE.Rules;
using AE.Source;
using AE.System;

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

    public async Task<int> RunAsync(CancellationToken SToken = default)
    {
        var TargetInfo = new TargetInfo()
        {
            BuildConfiguration = new()
            {
                Configuration = Configuration.Development,
                Platform = BuildHostPlatform.Current.Platform
            },
            bEditor = true
        };

        Workspace Workspace = new();
        await Workspace.ConfigureWorkspaceAsync(Global.EngineDirectory, true, SToken);
        if (GeneratorArgs.ProjectFile != null)
        {
            if (Path.IsPathFullyQualified(GeneratorArgs.ProjectFile) == false)
            {
                GeneratorArgs.ProjectFile = Path.Combine(Environment.CurrentDirectory, GeneratorArgs.ProjectFile);
            }

            if (File.Exists(GeneratorArgs.ProjectFile) == false)
            {
                throw new TerminateException(6, CoreStrings.Errors.InvalidProjectFormat);
            }

            string ProjectFileDir = Path.GetFullPath(Path.GetDirectoryName(GeneratorArgs.ProjectFile)!);
            ProjectDirectory GameProject = new() { Root = ProjectFileDir };
            await Workspace.ConfigureWorkspaceAsync(GameProject, false, SToken);
        }

        VisualStudioSolution Solution = new(Workspace, GeneratorArgs.ProjectFile);
        await Solution.GenerateProjectFilesAsync(SToken);
        return 0;
    }
}
