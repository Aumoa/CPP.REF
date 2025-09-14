using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

internal record SComment(CapturedContext Context, string Text) : Syntax(Context, null)
{
    public override string ToString()
    {
        return FormatLineNumber() + $"// {Text}";
    }

    public static bool TryAccept(Context context, [NotNullWhen(true)] out SComment? comment)
    {
        var capture = context.Capture();
        if (context.Current.StartsWith("//"))
        {
            var text = context.Export(2, "\n").Trim();
            comment = new SComment(capture, text.ToString());
            return true;
        }
        else if (context.Current.StartsWith("/*"))
        {
            var text = context.Export(2, "*/").Trim();
            comment = new SComment(capture, text.ToString());
            return true;
        }

        comment = null;
        return false;
    }
}
