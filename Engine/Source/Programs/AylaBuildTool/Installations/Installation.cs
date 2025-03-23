namespace AylaEngine;

internal abstract class Installation
{
    public abstract ValueTask<Compiler> SpawnCompilerAsync(TargetInfo targetInfo, CancellationToken cancellationToken);
    public abstract ValueTask<Linker> SpawnLinkerAsync(TargetInfo targetInfo, CancellationToken cancellationToken);
}
