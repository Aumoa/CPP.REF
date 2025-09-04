using System.Diagnostics.CodeAnalysis;
using static AylaEngine.RHTGenerator;

namespace AylaEngine.RHT.Syntaxes;

internal record SAClass(CapturedContext Context, SClass Class) : Syntax(Context, Class.EscapeBracket)
{
    private SGeneratedBody? m_Body;
    private readonly List<SProperty> m_Properties = [];
    private readonly List<SFunction> m_Functions = [];

    public override string ToString()
    {
        return FormatLineNumber() + "ACLASS()";
    }

    public SGeneratedBody Body
    {
        get => m_Body ?? throw Context.ParsingError("Syntax Error: GENERATED_BODY() is required in ACLASS().");
        set
        {
            if (m_Body != null)
            {
                throw Context.ParsingError("Syntax Error: Multiple GENERATED_BODY() in a single ACLASS() are not allowed.");
            }

            m_Body = value;
        }
    }

    public IReadOnlyList<SProperty> Properties => m_Properties;

    public void AddProperty(SProperty property)
    {
        m_Properties.Add(property);
    }

    public IReadOnlyList<SFunction> Functions => m_Functions;

    public void AddFunction(SFunction function)
    {
        m_Functions.Add(function);
    }

    public static bool TryAccept(Context context, List<Syntax> bracketStack, [NotNullWhen(true)] out SAClass? aclass)
    {
        if (context.WholeEquals("ACLASS()") == false)
        {
            aclass = null;
            return false;
        }

        var capture = context.Capture();
        context.Advance("ACLASS()".Length);

        context.SkipWhiteSpace(true);

        if (SClass.TryAccept(context, bracketStack, out var @class) == false)
        {
            throw context.ParsingError("A class definition is required after ACLASS().");
        }

        aclass = new SAClass(capture, @class);
        return true;
    }
}
