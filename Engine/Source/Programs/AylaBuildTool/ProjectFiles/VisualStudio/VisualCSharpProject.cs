// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.BuildSettings;
using AE.Misc;
using AE.Projects;

namespace AE.ProjectFiles.VisualStudio;

public class VisualCSharpProject : IVisualStudioProject
{
    public required ACSModule Module { get; init; }

    public string TargetName => Module.ModuleName;

    public required string ProjectGuid { get; init; }

    public required string ProjectFile { get; init; }

    public required string FilterPath { get; init; }

    [SetsRequiredMembers]
    public VisualCSharpProject(ACSModule Module)
    {
        this.Module = Module;
        ProjectGuid = CRC32.GenerateGuid(Module.SourcePath).ToString().ToUpper();
        ProjectFile = Path.Combine(Module.SourcePath, Path.ChangeExtension(Module.ModuleName, ".csproj"));

        if (Module.IsInProgramsDirectory())
        {
            FilterPath = "Programs";
        }
        else
        {
            if (Module.ProjectDirectory.Root == Global.EngineDirectory.Root)
            {
                FilterPath = "Engine";
            }
            else
            {
                FilterPath = "Game";
            }
        }
    }

    public (string, string) MapConfiguration(Configuration Configuration, bool bEditor, TargetPlatform Platform)
    {
        if (Configuration == Configuration.Debug)
        {
            return ("Debug", "Any CPU");
        }
        else
        {
            return ("Release", "Any CPU");
        }
    }

    public Task GenerateProjectFilesAsync(CancellationToken SToken = default)
    {
        return Task.CompletedTask;
    }
}
