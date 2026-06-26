using System.Collections.Generic;
using System.Runtime.ExceptionServices;
using System.Runtime.InteropServices;
using System.Threading;

namespace Ayla;

public static class ManagedExceptionInterop
{
    private static readonly object m_ManagedExceptionsLock = new();
    private static readonly Dictionary<ulong, ExceptionDispatchInfo> m_ManagedExceptions = [];
    private static long m_NextManagedExceptionToken;

    public static unsafe NativeCallStatus Capture(global::System.Exception exception)
    {
        string typeName = GetSafeTypeName(exception);
        string message = GetSafeMessage(exception);
        string details = GetSafeDetails(exception, typeName, message);
        ulong nativeExceptionToken = exception is NativeException nativeException ? nativeException.DetachNativeExceptionToken() : 0;
        ulong managedExceptionToken = 0;

        try
        {
            managedExceptionToken = nativeExceptionToken == 0 ? RegisterException(exception) : 0;

            fixed (char* typeNamePtr = typeName)
            fixed (char* messagePtr = message)
            fixed (char* detailsPtr = details)
            {
                return CaptureException__Injected(
                    new ManagedStringWrapper(typeNamePtr, typeName.Length),
                    new ManagedStringWrapper(messagePtr, message.Length),
                    new ManagedStringWrapper(detailsPtr, details.Length),
                    nativeExceptionToken,
                    managedExceptionToken);
            }
        }
        catch
        {
            NativeException.ReleaseCaptured(nativeExceptionToken);
            ReleaseCaptured(managedExceptionToken);
            throw;
        }
    }

    private static string GetSafeTypeName(global::System.Exception exception)
    {
        try
        {
            var type = exception.GetType();
            return type.FullName ?? type.Name;
        }
        catch
        {
            return "System.Exception";
        }
    }

    private static string GetSafeMessage(global::System.Exception exception)
    {
        try
        {
            return exception.Message ?? string.Empty;
        }
        catch
        {
            return "Failed to capture managed exception message.";
        }
    }

    private static string GetSafeDetails(global::System.Exception exception, string typeName, string message)
    {
        try
        {
            return exception.ToString();
        }
        catch
        {
            return $"{typeName}: {message}";
        }
    }

    internal static void ThrowCaptured(ulong managedExceptionToken)
    {
        if (managedExceptionToken == 0)
        {
            return;
        }

        ExceptionDispatchInfo? dispatchInfo;
        lock (m_ManagedExceptionsLock)
        {
            if (m_ManagedExceptions.Remove(managedExceptionToken, out dispatchInfo) == false || dispatchInfo == null)
            {
                throw new InvalidOperationException($"Managed exception token {managedExceptionToken} is not registered.");
            }
        }

        dispatchInfo.Throw();
    }

    internal static void ReleaseCaptured(ulong managedExceptionToken)
    {
        if (managedExceptionToken == 0)
        {
            return;
        }

        lock (m_ManagedExceptionsLock)
        {
            m_ManagedExceptions.Remove(managedExceptionToken);
        }
    }

    private static ulong RegisterException(global::System.Exception exception)
    {
        ulong managedExceptionToken;
        do
        {
            managedExceptionToken = unchecked((ulong)Interlocked.Increment(ref m_NextManagedExceptionToken));
        }
        while (managedExceptionToken == 0);

        lock (m_ManagedExceptionsLock)
        {
            m_ManagedExceptions.Add(managedExceptionToken, ExceptionDispatchInfo.Capture(exception));
        }

        return managedExceptionToken;
    }

    [DllImport("Core", EntryPoint = "Ayla__ManagedExceptionInterop__CaptureException__Injected")]
    private static extern NativeCallStatus CaptureException__Injected(
        ManagedStringWrapper typeName,
        ManagedStringWrapper message,
        ManagedStringWrapper details,
        ulong nativeExceptionToken,
        ulong managedExceptionToken);
}
