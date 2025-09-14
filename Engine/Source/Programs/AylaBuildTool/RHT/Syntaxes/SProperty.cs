using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

internal record SProperty(CapturedContext Context, SVariable Variable) : Syntax(Context, null)
{
    public override string ToString()
    {
        return FormatLineNumber() + "APROPERTY()";
    }

    public static bool TryAccept(Context context, [NotNullWhen(true)] out SProperty? aproperty)
    {
        if (context.WholeEquals("APROPERTY()") == false)
        {
            aproperty = null;
            return false;
        }

        var capture = context.Capture();
        context.Advance("APROPERTY()".Length);
        context.SkipWhiteSpace(true);
        var vinfo = SVariable.Accept(context, c => c == ';');
        aproperty = new SProperty(capture, vinfo);
        return true;
    }
}
