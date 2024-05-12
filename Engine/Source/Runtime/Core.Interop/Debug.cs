using System.Diagnostics;
using SystemDebug = System.Diagnostics.Debug;

namespace AylaEngine;

public static class Debug
{
    public static void LogFormat(string format, params object[] args)
    {
#if DEBUG
        SystemDebug.WriteLine(format, args);
#endif
    }

    public static void LogErrorFormat(string format, params object[] args)
    {
#if DEBUG
        SystemDebug.WriteLine(format, args);
#endif
    }
}
