namespace AylaEngine;

internal abstract class Linker
{
    public abstract ValueTask<Terminal.Output> LinkAsync(ModuleRulesResolver module, CppCompiler.CompileItem[] sourceObjects, CancellationToken cancellationToken);
}
