// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Projects;
using AE.Rules;

namespace AE.Platform;

public abstract class Linker
{
    public abstract Task<string> LinkAsync(ModuleInformation Module, CancellationToken SToken = default);

    public abstract Task<bool> TryCheckOutputsAsync(ModuleInformation Module, CancellationToken SToken = default);
}
