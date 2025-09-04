using System.Diagnostics.CodeAnalysis;
using static AylaEngine.RHTGenerator;

namespace AylaEngine.RHT.Syntaxes;

internal record SGeneratedBody(CapturedContext Context) : Syntax(Context, null)
{
    public override string ToString()
    {
        return FormatLineNumber() + "GENERATED_BODY()";
    }

    public static bool TryAccept(Context context, [NotNullWhen(true)] out SGeneratedBody? generatedBody)
    {
        if (context.WholeEquals("GENERATED_BODY()") == false)
        {
            generatedBody = null;
            return false;
        }

        var capture = context.Capture();
        context.Advance("GENERATED_BODY()".Length);
        generatedBody = new SGeneratedBody(capture);
        return true;
    }
}
