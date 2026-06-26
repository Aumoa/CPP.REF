using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

public static class Debug
{
    [DebuggerHidden]
    private static unsafe void Log__Internal(string category, LogVerbosity logLevel, string message)
    {
        fixed (char* categoryPtr = category)
        fixed (char* messagePtr = message)
        {
            var category_wrapper = new ManagedStringWrapper(categoryPtr, category.Length);
            var message_wrapper = new ManagedStringWrapper(messagePtr, message.Length);
            NativeCallBoundary.ThrowIfFailed(Ayla__Debug__Log__Injected(category_wrapper, logLevel, message_wrapper));
        }

        return;

        [DllImport("Core")]
        static extern NativeCallStatus Ayla__Debug__Log__Injected(ManagedStringWrapper category, LogVerbosity logLevel, ManagedStringWrapper message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void Log(string category, LogVerbosity logLevel, string message)
    {
        Log__Internal(category, logLevel, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void Log(string category, LogVerbosity logLevel, string format, params ReadOnlySpan<object?> args)
    {
        Log(category, logLevel, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogVerbose(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Verbose, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogVerbose(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Verbose, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogInfo(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Info, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogInfo(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Info, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogWarning(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Warning, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogWarning(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Warning, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogError(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Error, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogError(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Error, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogCritical(string category, string message)
    {
        Log__Internal(category, LogVerbosity.Critical, message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [DebuggerHidden]
    public static void LogCritical(string category, string format, params ReadOnlySpan<object?> args)
    {
        Log__Internal(category, LogVerbosity.Critical, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [Conditional("DO_CHECK")]
    [DebuggerHidden]
    public static unsafe void Ensure([DoesNotReturnIf(false)] bool condition, [CallerArgumentExpression("condition")] string message = null!)
    {
        fixed (char* messagePtr = message)
        {
            var message_wrapper = new ManagedStringWrapper(messagePtr, message.Length);
            Ayla__Debug__EnsureMessage__Injected(condition, message_wrapper);
        }

        return;

        [DllImport("Core")]
        static extern void Ayla__Debug__EnsureMessage__Injected(bool condition, ManagedStringWrapper message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [Conditional("DO_CHECK")]
    [DebuggerHidden]
    public static void EnsureFormat([DoesNotReturnIf(false)] bool condition, [StringSyntax("CompositeFormat")] string format, params scoped ReadOnlySpan<object?> args)
    {
        Ensure(condition, string.Format(format, args));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [Conditional("DO_CHECK")]
    [DebuggerHidden]
    public static unsafe void Assert([DoesNotReturnIf(false)] bool condition, [CallerArgumentExpression("condition")] string message = null!)
    {
        fixed (char* messagePtr = message)
        {
            var message_wrapper = new ManagedStringWrapper(messagePtr, message.Length);
            Ayla__Debug__AssertMessage__Injected(condition, message_wrapper);
        }

        return;

        [DllImport("Core")]
        static extern void Ayla__Debug__AssertMessage__Injected(bool condition, ManagedStringWrapper message);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    [Conditional("DO_CHECK")]
    [DebuggerHidden]
    public static void AssertFormat([DoesNotReturnIf(false)] bool condition, [StringSyntax("CompositeFormat")] string format, params scoped ReadOnlySpan<object?> args)
    {
        Assert(condition, string.Format(format, args));
    }
}
