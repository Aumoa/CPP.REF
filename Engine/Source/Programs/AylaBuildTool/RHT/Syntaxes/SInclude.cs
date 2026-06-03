using System.Diagnostics.CodeAnalysis;

namespace AylaEngine.RHT;

internal record SInclude(CapturedContext Context, string Path) : Syntax(Context, null)
{
    public override string ToString()
    {
        return FormatLineNumber() + $"#include \"{Path}\"";
    }

    public static bool TryAccept(Context context, [NotNullWhen(true)] out SInclude? include)
    {
        if (context.Current.StartsWith("#include") == false)
        {
            include = null;
            return false;
        }

        var capture = context.Capture();
        context.Advance("#include".Length);
        context.SkipWhiteSpace(false);

        ReadOnlySpan<char> escape;
        switch (context.CurrentChar)
        {
            case '<':
                escape = ">";
                break;
            case '\"':
                escape = "\"";
                break;
            default:
                throw capture.ParsingError("Syntax Error: Expected '<' or '\"' but found an unexpected character.");
        }

        var includePath = context.Export(1, escape).Trim();
        include = new SInclude(capture, includePath.ToString());
        return true;
    }
}
