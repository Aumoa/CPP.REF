// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

namespace DotNETUtilities.Script;

class HtmlScriptWriter : ScriptWriter
{
    int? _StartStringIndex;

    public HtmlScriptWriter(ConsoleScriptContext InContext) : base(InContext)
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
            '<' or '>' => new StringScriptWriter(Context),
            _ => this
        };

        if (NextWriter != this)
        {
            string Scoped = Context.Format[_StartStringIndex.Value..IndexOf];
            if (string.IsNullOrEmpty(Scoped))
            {
                _StartStringIndex = null;
                return NextWriter;
            }

            bool bEscape = false;
            if (Scoped.Length == 0)
            {
                ThrowException(IndexOf, Scoped);
            }

            // Escape sequence.
            if (Scoped[0] == '/')
            {
                Scoped = Scoped[1..];
                bEscape = true;
            }

            string? Value = null;
            int IndexOfSplit = Scoped.IndexOf('=');
            if (IndexOfSplit != -1)
            {
                Value = Scoped[(IndexOfSplit + 1)..];
                Scoped = Scoped[..IndexOfSplit];
            }

            if (Scoped.Length == 0)
            {
                ThrowException(IndexOf, Scoped);
            }

            switch (Scoped.ToLower())
            {
                case "color":
                    ScriptColor(Value, bEscape);
                    break;
                default:
                    ThrowException(IndexOf, Scoped);
                    break;
            }

            _StartStringIndex = null;
        }

        return NextWriter;
    }

    private void ScriptColor(string? InValue, bool bEscape)
    {
        if (bEscape)
        {
            if (!Context.ForegroundColorStack.TryPop(out _))
            {
                throw new FormatException("Color 서식 지정자가 짝을 이루지 못했습니다.");
            }

            if (!Context.ForegroundColorStack.TryPeek(out ConsoleColor Color))
            {
                throw new FormatException("Color 서식 지정자가 짝을 이루지 못했습니다.");
            }

            Console.ForegroundColor = Color;
        }
        else
        {
            if (!Enum.TryParse(typeof(ConsoleColor), InValue, out var EnumObject))
            {
                throw new FormatException($"Color 서식 지정자에서 \"{InValue}\" 값은 올바르지 않습니다. {nameof(ConsoleColor)} 열거형을 참고하세요.");
            }

            var Color = (ConsoleColor)EnumObject!;
            Context.ForegroundColorStack.Push(Color);
            Console.ForegroundColor = Color;
        }
    }

    public override void Dispose()
    {
        if (_StartStringIndex.HasValue)
        {
            throw new FormatException("서식 지정자가 종료되지 않았습니다. 서식 지정자(<)로 시작한 문자열은 종료자(>)로 끝나야 합니다.");
        }
    }

    [DoesNotReturn]
    private static void ThrowException(int IndexOf, string InScopedText)
    {
        throw new FormatException($"서식 지정자가 올바르지 않습니다. 텍스트 위치: {IndexOf}, 범위 텍스트: \"{InScopedText}\"");
    }
}
