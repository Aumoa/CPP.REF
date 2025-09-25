namespace AylaEngine;

public abstract class CSSourceCode
{
    public abstract ValueTask<string> GetIdentifierAsync(CancellationToken cancellationToken = default);

    public abstract long GetETag();

    public abstract ValueTask<string> ReadContentAsync(CancellationToken cancellationToken = default);
}
