﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.Exceptions;
using AE.IO;
using AE.Projects;
using AE.Source;

namespace AE.Executors;

public abstract class ProjectBasedExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "ProjectFile")]
        public string? ProjectFile { get; set; }
    }

    private readonly Arguments projectBasedArgs = new();

    public ProjectBasedExecutor(CommandLineParser args)
    {
        args.ApplyTo(projectBasedArgs);
    }

    protected void ConfigureWorkspace()
    {
        if (string.IsNullOrEmpty(projectBasedArgs.ProjectFile))
        {
            Workspace.Init(Global.EngineDirectory, null);
        }
        else
        {
            var projectFile = new FileReference(projectBasedArgs.ProjectFile);

            if (projectFile.IsPathFullQualified == false)
            {
                projectFile = Path.Combine(Environment.CurrentDirectory, projectBasedArgs.ProjectFile);
            }

            if (File.Exists(projectBasedArgs.ProjectFile) == false)
            {
                throw new TerminateException(KnownErrorCode.TargetNotFound, CoreStrings.Errors.InvalidProjectFormat);
            }

            string projectFileDir = Path.GetFullPath(Path.GetDirectoryName(projectBasedArgs.ProjectFile)!);
            ProjectDirectory gameProject = new() { Root = projectFileDir };
            Workspace.Init(gameProject, projectBasedArgs.ProjectFile);
        }
    }
}
