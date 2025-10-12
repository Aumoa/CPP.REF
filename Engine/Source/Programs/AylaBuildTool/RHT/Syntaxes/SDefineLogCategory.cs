using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

internal record SDefineLogCategory(CapturedContext Context, SNamespace[] Namespaces, string Name) : Syntax(Context, null)
{
    public override string ToString()
    {
        return FormatLineNumber() + $"DEFINE_LOG_CATEGORY({Name});";
    }

    public static bool TryAccept(Context context, IReadOnlyList<Syntax> bracketStack, [NotNullWhen(true)] out SDefineLogCategory? defineLogCategory)
    {
        if (context.Current.StartsWith("DEFINE_LOG_CATEGORY") == false)
        {
            defineLogCategory = null;
            return false;
        }

        var capture = context.Capture();
        context.Advance("DEFINE_LOG_CATEGORY".Length);
        context.SkipWhiteSpace(false);

        if (context.CurrentChar != '(')
        {
            throw capture.ParsingError("Syntax Error: Expected '(' after 'DEFINE_LOG_CATEGORY'.");
        }

        var categoryName = context.Export(1, ")").Trim();
        if (categoryName.Length == 0)
        {
            throw capture.ParsingError("Syntax Error: Log category name is empty.");
        }

        context.Advance(1);
        context.SkipWhiteSpace(false);
        if (context.CurrentChar != ';')
        {
            throw capture.ParsingError("Syntax Error: Expected ';' after log category name.");
        }
        context.Advance(1);

        defineLogCategory = new SDefineLogCategory(capture, bracketStack.OfType<SNamespace>().ToArray(), categoryName.ToString());
        return true;
    }
}