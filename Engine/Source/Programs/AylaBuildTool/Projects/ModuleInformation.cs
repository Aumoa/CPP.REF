// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Rules;
using AE.Source;

namespace AE.Projects;

public record ModuleInformation
{
    public required string Name { get; init; }

    public required ProjectDirectory ProjectDir { get; init; }

    public required TargetType TargetType { get; init; }

    public required string[] IncludePaths { get; init; }

    public required string[] AdditionalMacros { get; init; }

    public required string[] SourceFiles { get; set; }

    public required string SourcePath { get; init; }

    public required string GeneratedIncludePath { get; init; }

    public required string[] DependModules { get; init; }

    public required string[] AdditionalLibraries { get; init; }

    public required int[] DisableWarnings { get; init; }
}
