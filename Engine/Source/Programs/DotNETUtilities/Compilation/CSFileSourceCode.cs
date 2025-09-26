
namespace AylaEngine;

public class CSFileSourceCode : CSSourceCode
{
    private readonly string m_FilePath;

    public CSFileSourceCode(string filePath)
    {
        m_FilePath = filePath;
    }

    public override ValueTask<string> GetIdentifierAsync(CancellationToken cancellationToken = default)
    {
        return ValueTask.FromResult(m_FilePath);
    }

    public override long GetETag()
    {
        return File.Exists(m_FilePath) ? File.GetLastWriteTimeUtc(m_FilePath).ToBinary() : 0;
    }

    public override async ValueTask<string> ReadContentAsync(CancellationToken cancellationToken = default)
    {
        return await File.ReadAllTextAsync(m_FilePath, cancellationToken);
    }

    public override ValueTask<CSSourceCode> InstantiateAsync(CancellationToken cancellationToken = default)
    {
        return ValueTask.FromResult<CSSourceCode>(this);
    }
}
