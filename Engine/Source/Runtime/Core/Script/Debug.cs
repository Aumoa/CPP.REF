using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

public static class Debug
{
    [DllImport("Core", EntryPoint = "Ayla__Debug__Log")]
    private static extern void Log__Injected(ManagedStringWrapper category, LogVerbosity logLevel, ManagedStringWrapper message);

    private static unsafe void Log__Internal(string category, LogVerbosity logLevel, string message)
    {
        fixed (char* categoryPtr = category)
        fixed (char* messagePtr = message)
        {
            var category_wrapper = new ManagedStringWrapper { C_str = (nint)categoryPtr, Length = category.Length };
            var message_wrapper = new ManagedStringWrapper { C_str = (nint)messagePtr, Length = message.Length };
            Log__Injected(category_wrapper, logLevel, message_wrapper);
        }
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void Log(string category, LogVerbosity logLevel, string message)
    {
        Log__Internal(category, logLevel, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void Log(string category, LogVerbosity logLevel, string format, params ReadOnlySpan<object?> args)
    {
        Log(category, logLevel, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogVerbose(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Verbose, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogVerbose(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Verbose, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogInformation(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Info, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogInformation(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Info, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogWarning(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Warning, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogWarning(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Warning, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogError(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Error, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogError(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Error, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogCritical(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Critical, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void LogCritical(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Critical, string.Format(format, args));
    }
}
