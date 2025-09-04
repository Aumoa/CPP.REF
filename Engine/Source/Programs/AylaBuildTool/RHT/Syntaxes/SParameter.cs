namespace AylaEngine.RHT.Syntaxes;

internal record SParameter(SVariable Variable) : Syntax(Variable.Context, null)
{
    public static SParameter Accept(Context context)
    {
        return new SParameter(SVariable.Accept(context, c => c == ',' || c == ')'));
    }
}
