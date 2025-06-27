namespace AylaEngine;

internal abstract class Generator
{
    public abstract ValueTask GenerateAsync(Solution solution, CancellationToken cancellationToken = default);
}
