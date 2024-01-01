// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Rules;
using AE.Source;

namespace AE.Projects;

public record SearchedModule
{
    public required ModuleRules Rules { get; init; }

    public required ProjectDirectory ProjectDirectory { get; init; }

    public required string SourcePath { get; init; }
}
