namespace AylaEngine;

public abstract class CSSourceCode
{
    public abstract ValueTask<string> GetIdentifierAsync(CancellationToken cancellationToken = default);

    public abstract long GetETag();

    public abstract ValueTask<string> ReadContentAsync(CancellationToken cancellationToken = default);

    public static CSFileSourceCode FromFile(string filePath) => new CSFileSourceCode(filePath);

    public static CSIntSourceCode FromString(string identifier, string sourceCodeText) => new CSIntSourceCode(identifier, sourceCodeText);
}
