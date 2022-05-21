// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities.Script;

class StringScriptWriter : ScriptWriter
{
    int? _StartStringIndex;

    public StringScriptWriter(ConsoleScriptContext InContext) : base(InContext)
    {
    }

    public override ScriptWriter StreamIn(int IndexOf)
    {
        if (_StartStringIndex == null)
        {
            _StartStringIndex = IndexOf;
        }

        char Ch = Context.Format![IndexOf];
        ScriptWriter NextWriter = Ch switch
        {
            '\"' => new TextScriptWriter(Context),
            '<' => new HtmlScriptWriter(Context),
            _ => this
        };

        if (NextWriter != this)
        {
            // Flush all string without current character.
            Console.Write(Context.Format[_StartStringIndex.Value..IndexOf]);
            _StartStringIndex = null;
        }

        return NextWriter;
    }

    public override void Dispose()
    {
        if (_StartStringIndex.HasValue)
        {
            // Flush all string.
            Console.Write(Context.Format![_StartStringIndex.Value..^0]);
        }
    }
}
