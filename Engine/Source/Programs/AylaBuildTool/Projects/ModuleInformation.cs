// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public record ModuleInformation
{
    public required string Name { get; init; }

    public required ProjectDirectory ProjectDir { get; init; }

    public required ModuleRules.ModuleType TargetType { get; init; }

    public required FileReference[] SourceFiles { get; set; }

    public required string SourcePath { get; init; }

    public string ScriptPath => Path.Combine(SourcePath, "Scripts");

    public required string GeneratedIncludePath { get; init; }

    public required string CSharpPath { get; init; }

    public required string GeneratedShaderPath { get; init; }

    public required string[] DependModules { get; init; }

    public required string[] PublicIncludePaths { get; init; }

    public required string[] PrivateIncludePaths { get; init; }

    public required string[] PublicAdditionalMacros { get; init; }

    public required string[] PrivateAdditionalMacros { get; init; }

    public required string[] PublicAdditionalLibraries { get; init; }

    public required string[] PrivateAdditionalLibraries { get; init; }

    public required int[] PublicDisableWarnings { get; init; }

    public required int[] PrivateDisableWarnings { get; init; }
}
