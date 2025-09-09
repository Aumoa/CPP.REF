namespace AylaEngine.RHT.Syntaxes;

internal abstract record SMember(CapturedContext Context, char? EscapeBracket, string Name) : Syntax(Context, EscapeBracket)
{
}
