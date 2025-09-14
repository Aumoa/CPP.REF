using System.Diagnostics.CodeAnalysis;
using static AylaEngine.RHTGenerator;

namespace AylaEngine;

internal record SNamespace(CapturedContext Context, string Name) : Syntax(Context, '}')
{
    public override string ToString()
    {
        return FormatLineNumber() + $"namespace {Name}";
    }

    public static bool TryAccept(Context context, [NotNullWhen(true)] out SNamespace? @namespace)
    {
        if (context.WholeEquals("namespace") == false)
        {
            @namespace = null;
            return false;
        }

        var capture = context.Capture();
        context.WholeAdvance("namespace");
        context.SkipWhiteSpace(true);
        var name = context.Export(0, "{").Trim();
        @namespace = new SNamespace(capture, name.ToString());
        return true;
    }
}
