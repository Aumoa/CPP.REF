namespace AylaEngine.RHT.Syntaxes;

internal interface ICapturedContext
{
    string FilePath { get; }

    int LineNumber { get; }

    int ColumnNumber { get; }
}
