namespace AylaEngine;

internal abstract class Linker
{
    public abstract ValueTask<Terminal.Output> LinkAsync(ModuleRulesResolver module, Compiler.CompileItem[] sourceObjects, CancellationToken cancellationToken);
}
