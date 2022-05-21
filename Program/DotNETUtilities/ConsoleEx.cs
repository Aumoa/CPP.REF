// Copyright 2020-2022 Aumoa.lib. All right reserved.

using DotNETUtilities.Script;

namespace DotNETUtilities;

/// <summary>
/// 콘솔 화면에 대한 입/출력을 제어합니다. 전용 스레드를 사용합니다.
/// </summary>
public static class ConsoleEx
{
    internal static void WriteFormatted(ConsoleCursorPin pin, string formatted, params object[] args)
    {
        if (args.Any())
        {
            formatted = string.Format(formatted, args);
        }

        (int, int)? restoreContext = null;
        if (pin != null)
        {
            restoreContext = pin.InternalMoveCursor();
        }

        ConsoleScriptContext Context = new()
        {
            Format = formatted,
        };

        Context.CurrentWriter = new StringScriptWriter(Context);
        Context.ForegroundColorStack.Push(Console.ForegroundColor);

        for (int i = 0; i < formatted.Length; ++i)
        {
            ScriptWriter NextWriter = Context.CurrentWriter.StreamIn(i);
            if (NextWriter != Context.CurrentWriter)
            {
                Context.CurrentWriter.Dispose();
                Context.CurrentWriter = NextWriter;
            }
        }

        Context.CurrentWriter?.Dispose();
        Console.ForegroundColor = Context.ForegroundColorStack.Pop();

        ConsoleCursorPin.RestoreCursor(restoreContext!.Value);
    }
}
