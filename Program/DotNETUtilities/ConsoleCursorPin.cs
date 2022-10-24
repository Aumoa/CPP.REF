// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

/// <summary>
/// 고정된 콘솔의 위치를 기록하는 개체를 표현합니다.
/// </summary>
public class ConsoleCursorPin
{
    private int? _top;
    private int _height;

    private ConsoleCursorPin()
    {
    }

    /// <summary>
    /// 콘솔의 현재 위치를 기록하고 <paramref name="height"/>만큼 개행합니다.
    /// </summary>
    /// <param name="height"> 예약된 줄 개수를 전달합니다. </param>
    /// <returns> 현재 위치를 나타내는 개체가 반환됩니다. </returns>
    public static ConsoleCursorPin Create(int height = 1)
    {
        var pin = new ConsoleCursorPin()
        {
            _height = height
        };

        Log.Action(() =>
        {
            pin._top = SafeCursorTop;
            Console.Write(new string('\n', height));
        });

        return pin;
    }

    internal (int, int) InternalMoveCursor()
    {
        (int, int) restoreContext = (SafeCursorLeft, SafeCursorTop);
        SafeCursorLeft = 0;
        SafeCursorTop = _top!.Value;
        return restoreContext;
    }

    internal static void RestoreCursor((int, int) restoreContext)
    {
        SafeCursorLeft = restoreContext.Item1;
        SafeCursorTop = restoreContext.Item2;
    }

    internal static int SafeCursorLeft
    {
        get
        {
            try
            {
                return Console.CursorLeft;
            }
            catch (IOException)
            {
                return 0;
            }
        }
        set
        {
            try
            {
                Console.CursorLeft = value;
            }
            catch (IOException)
            {
            }
        }
    }

    internal static int SafeCursorTop
    {
        get
        {
            try
            {
                return Console.CursorTop;
            }
            catch (IOException)
            {
                return 0;
            }
        }
        set
        {
            try
            {
                Console.CursorTop = value;
            }
            catch (IOException)
            {
            }
        }
    }

    internal (int, int) InternalClear()
    {
        (int, int) restoreContext = InternalMoveCursor();

        int width = Console.BufferWidth;
        string pad = "".PadRight(width, ' ');
        for (int i = 0; i < _height; ++i)
        {
            Console.WriteLine(pad);
        }

        return restoreContext;
    }
}
