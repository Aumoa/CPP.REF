namespace AylaEngine;

internal record SAEnum(CapturedContext Context) : Syntax(Context, '}')
{
    public override string ToString()
    {
        return FormatLineNumber() + "AENUM()";
    }
}
