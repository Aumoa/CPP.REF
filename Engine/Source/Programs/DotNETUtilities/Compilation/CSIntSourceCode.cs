
namespace AylaEngine;

public class CSIntSourceCode(string identifier, string sourceCode) : CSSourceCode
{
    public override ValueTask<string> GetIdentifierAsync(CancellationToken cancellationToken = default)
    {
        return ValueTask.FromResult(identifier);
    }

    public override long GetETag()
    {
        unchecked
        {
            return (long)CRC64.Generate64(sourceCode);
        }
    }

    public override ValueTask<string> ReadContentAsync(CancellationToken cancellationToken = default)
    {
        return ValueTask.FromResult(sourceCode);
    }
}
