using System;
using System.Diagnostics.CodeAnalysis;
using static AylaEngine.RHTGenerator;

namespace AylaEngine.RHT.Syntaxes;

internal record SClass(
    CapturedContext Context,
    string Name,
    string? DllSpec,
    char? EscapeBracket,
    SNamespace[] Namespaces,
    STypeName? Base
    ) : Syntax(Context, EscapeBracket)
{
    public override string ToString()
    {
        if (DllSpec == null)
        {
            return FormatLineNumber() + $"class {Name}";
        }
        else
        {
            return FormatLineNumber() + $"class {DllSpec} {Name}";
        }
    }

    public static bool TryAccept(Context context, IReadOnlyList<Syntax> bracketStack, [NotNullWhen(true)] out SClass? @class)
    {
        if (context.WholeEquals("class") == false)
        {
            @class = null;
            return false;
        }

        context.WholeAdvance("class");
        context.SkipWhiteSpace(true);
        ReadOnlySpan<char> name;
        var capture = context.Capture();
        char? escapeBracket = null;
        string? @base = null;
        CapturedContext baseContext = default;
        switch (context.SelectExport(0, "{", ";", ":"))
        {
            case 0:
                name = context.Export(0, "{").Trim();
                escapeBracket = '}';
                break;
            case 1:
                name = context.Export(0, ";").Trim();
                break;
            case 2:
                name = context.Export(0, ":").Trim();
                baseContext = context.Capture();
                var inheritanceStr = context.Export(0, "{");
                var inheritances = inheritanceStr.ToString().Split(',', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                @base = inheritances[0].Replace("public ", string.Empty);
                escapeBracket = '}';
                break;
            default:
                throw context.ParsingError("Syntax Error: Expected '{' or ';' but found an unexpected character.");
        }

        var values = name.ToString().Split(' ', '\n', '\t', '\r');
        string? dllspec;
        string clsname;
        if (values.Length >= 2)
        {
            dllspec = values[0];
            clsname = values[1];
        }
        else
        {
            dllspec = null;
            clsname = name.ToString();
        }

        @class = new SClass(capture, clsname, dllspec, escapeBracket, bracketStack.OfType<SNamespace>().ToArray(), @base == null ? null : new STypeName(baseContext, [new STypeName.Part(STypeName.PartType.Name, @base)]));
        return true;
    }
}
