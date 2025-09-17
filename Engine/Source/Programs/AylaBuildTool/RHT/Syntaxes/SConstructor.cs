using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

internal record SConstructor(CapturedContext Context, string Name, SParameter[] Parameters, SAccessSpecifier.Types Access)
    : SMember(Context, null, Name, Access)
{
    public override string ToString()
    {
        return FormatLineNumber() + "AFUNCTION()";
    }

    public static bool TryAccept(Context context, List<Syntax> syntaxes, [NotNullWhen(true)] out SConstructor? constructor)
    {
        if (context.WholeEquals("ACONSTRUCTOR()") == false)
        {
            constructor = null;
            return false;
        }

        var capture = context.Capture();
        var lastAccess = GetLastAccessSpecifier(capture, syntaxes);
        context.Advance("ACONSTRUCTOR()".Length);
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

        context.ExportWhile(c => c != ';' && c != '{');
        constructor = new SConstructor(capture, name.ToString(), parameters.ToArray(), lastAccess);
        return true;

        bool IsNumber(char c) => c >= '0' && c <= '9';
        bool IsNameChar(char c) => (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || IsNumber(c) || c == '_';
    }
}
