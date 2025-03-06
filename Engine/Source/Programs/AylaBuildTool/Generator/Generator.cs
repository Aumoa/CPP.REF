namespace AylaEngine;

internal abstract class Generator
{
    public abstract Task GenerateAsync(Solution solution, CancellationToken cancellationToken = default);
}
