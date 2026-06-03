namespace AylaEngine.RHT;

internal interface ICapturedContext
{
    string FilePath { get; }

    int LineNumber { get; }

    int ColumnNumber { get; }
}
