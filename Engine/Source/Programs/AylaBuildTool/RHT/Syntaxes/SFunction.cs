using System.Diagnostics.CodeAnalysis;
using static AylaEngine.RHTGenerator;

namespace AylaEngine;

internal record SFunction(CapturedContext Context, string Name, SFunction.FFlags Flags, STypeName ReturnType, SParameter[] Parameters) : SMember(Context, null, Name)
{
    [Flags]
    public enum FFlags
    {
        None = 0,
        Static = 1 << 0,
        Virtual = 1 << 1,
        Const = 1 << 2
    }

    public override string ToString()
    {
        return FormatLineNumber() + "AFUNCTION()";
    }

    public static bool TryAccept(Context context, [NotNullWhen(true)] out SFunction? afunction)
    {
        if (context.WholeEquals("AFUNCTION()") == false)
        {
            afunction = null;
            return false;
        }

        var capture = context.Capture();
        context.Advance("AFUNCTION()".Length);
        context.SkipWhiteSpace(true);
        FFlags flags = FFlags.None;

        if (context.WholeEquals("virtual"))
        {
            flags |= FFlags.Virtual;
            context.WholeAdvance("virtual");
        }

        if (context.WholeEquals("static"))
        {
            flags |= FFlags.Static;
            context.WholeAdvance("static");
        }

        var returnType = STypeName.Accept(context);
        context.SkipWhiteSpace(true);

        if (IsNumber(context.CurrentChar))
        {
            throw capture.ParsingError("Syntax Error: A function name cannot start with a number.");
        }

        var name = context.ExportWhile(c => IsNameChar(c));
        context.SkipWhiteSpace(true);

        if (context.CurrentChar != '(')
        {
            throw capture.ParsingError("Syntax Error: Expected '(' but found an unexpected character.");
        }

        context.WholeAdvance("(");
        List<SParameter> parameters = [];
        while (context.CurrentChar != ')')
        {
            parameters.Add(SParameter.Accept(context));
            context.SkipWhiteSpace(true);
            if (context.CurrentChar == ',')
            {
                context.WholeAdvance(",");
                context.SkipWhiteSpace(true);
            }
        }

        if (context.WholeEquals("const"))
        {
            flags |= FFlags.Const;
            context.WholeAdvance("const");
        }

        context.ExportWhile(c => c != ';' && c != '{');
        afunction = new SFunction(capture, name.ToString(), flags, returnType, parameters.ToArray());
        return true;

        bool IsNumber(char c) => c >= '0' && c <= '9';
        bool IsNameChar(char c) => (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || IsNumber(c) || c == '_';
    }
}
