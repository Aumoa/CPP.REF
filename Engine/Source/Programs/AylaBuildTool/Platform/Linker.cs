// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public abstract class Linker
{
    public abstract Task<string> LinkAsync(ModuleInformation Module, CancellationToken SToken = default);

    public abstract Task<bool> TryCheckOutputsAsync(ModuleInformation Module, CancellationToken SToken = default);
}
