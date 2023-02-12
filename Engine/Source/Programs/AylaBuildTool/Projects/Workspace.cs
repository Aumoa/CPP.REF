// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Projects.VisualStudio;
using AE.Source;

namespace AE.Projects;

public class Workspace
{
    private readonly SourceCodeDirectory EngineDirectory;

    public string TargetName { get; private set; }

    public SourceCodeDirectory TargetDirectory => EngineDirectory;

    public Workspace(string EnginePath)
    {
        EngineDirectory = new SourceCodeDirectory(EnginePath);
        TargetName = "Engine";
    }

    public async Task GenerateDirectoriesAsync(CancellationToken CToken = default)
    {
        await EngineDirectory.GenerateDirectoriesAsync(CToken);
    }

    public async Task GenerateProjectFilesAsync(CancellationToken CToken = default)
    {
        VisualStudioSolution Solution = new(this);
        Solution.AddProject(new VisualCXXProject("AE", "Engine", EngineDirectory));
        foreach (var ProjectFile in Directory.GetFiles(EngineDirectory.ProgramsDirectory, "*.csproj", SearchOption.AllDirectories))
        {
            Solution.AddProject(new VisualCSharpProject(ProjectFile, "Programs"));
        }
        await Solution.GenerateProjectFilesAsync(CToken);
    }
}
