namespace AylaEngine;

internal abstract record Syntax(
    CapturedContext Context,
    char? EscapeBracket
    )
{
    public int LineNumber => Context.LineNumber;

    public int ColumnNumber => Context.ColumnNumber;

    protected string FormatLineNumber()
    {
        return $"({LineNumber},{ColumnNumber}): ";
    }
}
