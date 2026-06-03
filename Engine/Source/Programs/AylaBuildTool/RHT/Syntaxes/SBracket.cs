using System.Diagnostics.CodeAnalysis;
using static AylaEngine.RHT.RHTGenerator;

namespace AylaEngine.RHT;

internal record SBracket(CapturedContext Context, char? EscapeBracket) : Syntax(Context, EscapeBracket)
{
    public override string ToString()
    {
        return FormatLineNumber() + EscapeBracket switch
        {
            '}' => "{}",
            ']' => "[]",
            ')' => "()",
            _ => throw new InvalidOperationException($"Unsupported bracket escape character '{EscapeBracket}'.")
        };
    }

    public static bool TryAccept(Context context, [NotNullWhen(true)] out SBracket? unknownBracket)
    {
        if (context.CurrentChar == '{' || context.CurrentChar == '[' || context.CurrentChar == '(')
        {
            var capture = context.Capture();
            unknownBracket = new SBracket(capture, context.CurrentChar switch
            {
                '{' => '}',
                '[' => ']',
                '(' => ')',
                _ => throw new InvalidOperationException()
            });

            context.Advance(1);
            return true;
        }
        unknownBracket = null;
        return false;
    }
}
