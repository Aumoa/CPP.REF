// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities.Script;

class TextScriptWriter : ScriptWriter
{
    int? _StartStringIndex;

    public TextScriptWriter(ConsoleScriptContext InContext) : base(InContext)
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
            '\"' => new StringScriptWriter(Context),
            _ => this
        };

        if (NextWriter != this)
        {
            // Flush all string with quotation marks.
            Console.Write($"\"{Context.Format[_StartStringIndex.Value..IndexOf]}\"");
            _StartStringIndex = null;
        }

        return NextWriter;
    }

    public override void Dispose()
    {
        if (_StartStringIndex.HasValue)
        {
            throw new FormatException("문자열 범위가 종료되지 않았습니다. 문자열 마크(\")로 시작한 문자열은 문자열 마크를 통해 종료되어야 합니다.");
        }
    }
}
