namespace AylaEngine;

internal abstract class CppCompiler
{
    public readonly struct CompileItem
    {
        public ModuleRulesResolver Resolver { get; init; }

        public SourceCodeDescriptor SourceCode { get; init; }

        public GroupDescriptor Descriptor { get; init; }
    }

    public abstract ValueTask<Terminal.Output> CompileAsync(CompileItem item, CancellationToken cancellationToken);
}
