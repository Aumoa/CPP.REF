// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.Source;

namespace AE.Projects;

public class CSharpModule : Module
{
    [SetsRequiredMembers]
    public CSharpModule(ProjectDirectory ProjectDirectory, string SourceRelativePath)
    {
        string SourcePath = Path.Combine(ProjectDirectory.Source.Root, SourceRelativePath);
        this.Name = Path.GetFileName(SourcePath.Replace(".csproj", string.Empty));
        this.RuleName = this.Name;
        this.SourcePath = SourcePath;
        this.ProjectDirectory = ProjectDirectory;
    }

    public override string Name { get; }

    public required string RuleName { get; init; }

    public override string SourcePath { get; }

    public override ProjectDirectory ProjectDirectory { get; }

    public override string ToString()
    {
        return RuleName;
    }

    public bool IsInProgramsDirectory => SourcePath.StartsWith(ProjectDirectory.Source.Programs);
}
