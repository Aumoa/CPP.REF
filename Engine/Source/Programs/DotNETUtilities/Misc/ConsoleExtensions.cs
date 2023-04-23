// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Misc;

public static class ConsoleExtensions
{
    static ConsoleExtensions()
    {
        try
        {
            Console.GetCursorPosition();
            IsConsoleHandleSupports = true;
        }
        catch (IOException)
        {
            IsConsoleHandleSupports = false;
        }
    }

    public static bool IsConsoleHandleSupports { get; private set; }
}
