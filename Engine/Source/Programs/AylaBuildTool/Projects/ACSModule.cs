// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.Source;

namespace AE.Projects;

public class ACSModule : IAModule
{
    [SetsRequiredMembers]
    public ACSModule(ProjectDirectory ProjectDirectory, string SourceRelativePath)
    {
        string SourcePath = Path.Combine(ProjectDirectory.Source.Root, SourceRelativePath);
        this.ModuleName = Path.GetFileName(SourcePath.Replace(".csproj", string.Empty));
        this.RuleName = this.ModuleName;
        this.SourcePath = SourcePath;
        this.ProjectDirectory = ProjectDirectory;
    }

    public required string ModuleName { get; init; }

    public required string RuleName { get; init; }

    public required string SourcePath { get; init; }

    public required ProjectDirectory ProjectDirectory { get; init; }

    public override string ToString()
    {
        return RuleName;
    }

    public bool IsInProgramsDirectory => SourcePath.StartsWith(ProjectDirectory.Source.Programs);
}
