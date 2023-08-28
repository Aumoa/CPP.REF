// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Projects;
using AE.Rules;

namespace AE.Platform;

public abstract class Linker
{
    public abstract Task<string> LinkAsync(TargetRules Rule, ModuleInformation Module, CancellationToken SToken = default);
}
