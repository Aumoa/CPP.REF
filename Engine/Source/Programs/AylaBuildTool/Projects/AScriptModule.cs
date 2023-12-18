// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.Source;

namespace AE.Projects;

public class AScriptModule : IAModule
{
    [SetsRequiredMembers]
    public AScriptModule(ProjectDirectory ProjectDirectory, string CurrentDirectory)
    {
        var Components = CurrentDirectory.Split(Path.DirectorySeparatorChar);
        var ModuleDirectory = Path.Combine(Components[..^1]);

        this.ModuleName = Path.GetFileNameWithoutExtension(ModuleDirectory);
        this.SourcePath = CurrentDirectory;
        this.ProjectDirectory = ProjectDirectory;
    }

    public required string ModuleName { get; init; }

    public required string SourcePath { get; init; }

    public required ProjectDirectory ProjectDirectory { get; init; }

    public bool IsInProgramsDirectory => false;

    public override string ToString()
    {
        return $"Script \"{ModuleName}\"";
    }
}
