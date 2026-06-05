namespace AylaEngine.RHT;

internal abstract record SMember(CapturedContext Context, char? EscapeBracket, string Name, SAccessSpecifier.Types Access)
    : Syntax(Context, EscapeBracket)
{
    protected static SAccessSpecifier.Types GetLastAccessSpecifier<T>(T context, List<Syntax> syntaxes) where T : ICapturedContext
    {
        int lastClassIndex = syntaxes.FindLastIndex(p => p is SAClass);
        if (lastClassIndex == -1)
        {
            throw context.ParsingError("Syntax Error: APROPERTY() must be defined in ACLASS().");
        }

        return syntaxes
            .Skip(lastClassIndex)
            .OfType<SAccessSpecifier>()
            .LastOrDefault()?
            .Type ?? SAccessSpecifier.Types.Private;
    }
}
