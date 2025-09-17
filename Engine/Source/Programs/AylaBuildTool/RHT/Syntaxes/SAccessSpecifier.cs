using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

internal record SAccessSpecifier(CapturedContext Context, SAccessSpecifier.Types Type) : Syntax(Context, null)
{
    public enum Types
    {
        Public,
        Protected,
        Private
    }

    public override string ToString()
    {
        return FormatLineNumber() + $"{Type.ToString().ToLower()}:";
    }

    public static bool TryAccept(Context context, [NotNullWhen(true)] out SAccessSpecifier? accessSpecifier)
    {
        if (context.WholeEquals("public:"))
        {
            accessSpecifier = new SAccessSpecifier(context.Capture(), Types.Public);
            context.Advance("public:".Length);
            context.SkipWhiteSpace(true);
            return true;
        }
        else if (context.WholeEquals("protected:"))
        {
            accessSpecifier = new SAccessSpecifier(context.Capture(), Types.Protected);
            context.Advance("protected:".Length);
            context.SkipWhiteSpace(true);
            return true;
        }
        else if (context.WholeEquals("private:"))
        {
            accessSpecifier = new SAccessSpecifier(context.Capture(), Types.Private);
            context.Advance("private:".Length);
            context.SkipWhiteSpace(true);
            return true;
        }

        accessSpecifier = null;
        return false;
    }
}
