using System.Diagnostics.CodeAnalysis;

namespace AylaEngine.RHT;

internal record SProperty(CapturedContext Context, SVariable Variable, SAccessSpecifier.Types Access)
    : SMember(Context, null, Variable.Name, Access)
{
    public override string ToString()
    {
        return FormatLineNumber() + "APROPERTY()";
    }

    public static bool TryAccept(Context context, List<Syntax> syntaxes, [NotNullWhen(true)] out SProperty? aproperty)
    {
        if (context.WholeEquals("APROPERTY()") == false)
        {
            aproperty = null;
            return false;
        }

        var capture = context.Capture();
        var lastAccess = GetLastAccessSpecifier(capture, syntaxes);
        context.Advance("APROPERTY()".Length);
        context.SkipWhiteSpace(true);
        var vinfo = SVariable.Accept(context, c => c == ';');
        aproperty = new SProperty(capture, vinfo, lastAccess);
        return true;
    }
}
