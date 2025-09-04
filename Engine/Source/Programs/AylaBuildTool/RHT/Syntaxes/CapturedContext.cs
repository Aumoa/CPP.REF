namespace AylaEngine.RHT.Syntaxes;

internal record struct CapturedContext(
    string FilePath,
    int LineNumber,
    int ColumnNumber
    ) : ICapturedContext
{
}
