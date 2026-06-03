using System.Diagnostics.CodeAnalysis;

namespace AylaEngine.RHT;

internal record SAEnum(CapturedContext Context, string Name, SNamespace[] Namespaces, SEnumDefine[] Defines) : Syntax(Context, '}')
{
    public override string ToString()
    {
        return FormatLineNumber() + "AENUM()";
    }

    public static bool TryAccept(Context context, IReadOnlyList<Syntax> bracketStack, [NotNullWhen(true)] out SAEnum? @enum)
    {
        var captured = context.Capture();
        if (context.WholeEquals("AENUM()") == false)
        {
            @enum = null;
            return false;
        }
        context.WholeAdvance("AENUM");
        context.SkipWhiteSpace(true);
        if (context.CurrentChar != '(')
        {
            throw context.ParsingError("Syntax Error: Expected '(' after AENUM.");
        }
        context.Advance(1);
        context.SkipWhiteSpace(true);
        if (context.CurrentChar != ')')
        {
            throw context.ParsingError("Syntax Error: Expected ')' after AENUM(");
        }
        context.Advance(1);
        context.SkipWhiteSpace(true);
        if (context.WholeEquals("enum") == false)
        {
            throw context.ParsingError("Syntax Error: Expected 'enum' after AENUM().");
        }
        context.WholeAdvance("enum");
        if (context.WholeEquals("class"))
        {
            context.WholeAdvance("class");
        }
        else if (context.WholeEquals("struct"))
        {
            context.WholeAdvance("struct");
        }
        else
        {
            throw context.ParsingError("Syntax Error: Expected 'class' or 'struct' after 'enum'.");
        }

        var name = context.ExportWhile(c => c != '{').ToString().Trim();
        context.WholeAdvance("{");

        List<SEnumDefine> defines = [];
        while (context.IsEOF == false)
        {
            defines.Add(SEnumDefine.Accept(context));
            context.SkipWhiteSpace(true);
            if (context.CurrentChar == ',')
            {
                context.Advance(1);
                context.SkipWhiteSpace(true);
            }

            // Allow trailing comma before closing brace
            if (context.CurrentChar == '}')
            {
                break;
            }
        }

        if (context.CurrentChar != '}')
        {
            throw context.ParsingError("Syntax Error: Expected '}' to close enum definition.");
        }

        @enum = new SAEnum(captured, name, bracketStack.OfType<SNamespace>().ToArray(), defines.ToArray());
        return true;
    }
}
