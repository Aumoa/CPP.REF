// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.Exceptions;
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

    private readonly Arguments ProjectBasedArgs = new();

    public ProjectBasedExecutor(CommandLineParser Args)
    {
        Args.ApplyTo(ProjectBasedArgs);
    }

    protected async Task<Workspace> ConfigureWorkspaceAsync(CancellationToken SToken = default)
    {
        Workspace Workspace = new();
        await Workspace.ConfigureWorkspaceAsync(Global.EngineDirectory, true, SToken);
        if (ProjectBasedArgs.ProjectFile != null)
        {
            if (Path.IsPathFullyQualified(ProjectBasedArgs.ProjectFile) == false)
            {
                ProjectBasedArgs.ProjectFile = Path.Combine(Environment.CurrentDirectory, ProjectBasedArgs.ProjectFile);
            }

            if (File.Exists(ProjectBasedArgs.ProjectFile) == false)
            {
                throw new TerminateException(6, CoreStrings.Errors.InvalidProjectFormat);
            }

            string ProjectFileDir = Path.GetFullPath(Path.GetDirectoryName(ProjectBasedArgs.ProjectFile)!);
            ProjectDirectory GameProject = new() { Root = ProjectFileDir };
            await Workspace.ConfigureWorkspaceAsync(GameProject, false, SToken);
        }

        return Workspace;
    }

    protected ProjectDirectory[] GetProjectDirectories()
    {
        if (ProjectBasedArgs.ProjectFile != null)
        {
            if (Path.IsPathFullyQualified(ProjectBasedArgs.ProjectFile) == false)
            {
                ProjectBasedArgs.ProjectFile = Path.Combine(Environment.CurrentDirectory, ProjectBasedArgs.ProjectFile);
            }

            if (File.Exists(ProjectBasedArgs.ProjectFile) == false)
            {
                throw new TerminateException(6, CoreStrings.Errors.InvalidProjectFormat);
            }

            string ProjectFileDir = Path.GetFullPath(Path.GetDirectoryName(ProjectBasedArgs.ProjectFile)!);
            ProjectDirectory GameProject = new() { Root = ProjectFileDir };
            return new[] { Global.EngineDirectory, GameProject };
        }
        else
        {
            return new[] { Global.EngineDirectory };
        }
    }

    public string? ProjectFile => ProjectBasedArgs.ProjectFile;
}
