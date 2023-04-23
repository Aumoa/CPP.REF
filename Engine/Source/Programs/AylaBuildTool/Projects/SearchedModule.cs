// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Rules;
using AE.Source;

namespace AE.Projects;

public record SearchedModule
{
    public required ModuleRules Rule { get; init; }

    public required ProjectDirectory ProjectDir { get; init; }

    public required string Location { get; init; }
}
