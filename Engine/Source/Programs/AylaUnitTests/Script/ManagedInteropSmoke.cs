// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Collections;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla.Tests;

public static class ManagedInteropSmoke
{
    private const string ManagedExceptionMessage = "managed exception passport";
    private const string NativeExceptionMessage = "native exception passport";

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate int IntBinaryOperation(int left, int right);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate NativeCallStatus NativeStatusCallback();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate NativeCallStatus NativeStatusBridge(nint callback);

    private sealed class ManagedInteropSmokeException(string message, int marker) : InvalidOperationException(message)
    {
        public int Marker { get; } = marker;
    }

    private static int s_Counter;
    private static int s_ObservedNativeExceptionWrapper;
    private static ManagedInteropSmokeException? s_LastManagedException;
    private static readonly NativeStatusCallback s_ThrowManagedExceptionCallback = ThrowManagedExceptionCallback;

    public static int Add(int left, int right)
    {
        return left + right;
    }

    public static int InvokeNativeCallback(nint callback, int left, int right)
    {
        var operation = Marshal.GetDelegateForFunctionPointer<IntBinaryOperation>(callback);
        return operation(left, right);
    }

    public static int ResetCounter(int value)
    {
        s_Counter = value;
        return s_Counter;
    }

    public static int IncrementCounter(int delta)
    {
        s_Counter += delta;
        return s_Counter;
    }

    public static int InvokeCoreDebugLog()
    {
        Ayla.Debug.LogInfo("ManagedInterop", "Core debug log boundary smoke");
        return 1;
    }

    public static int RoundTripManagedExceptionThroughNative(nint nativeBridge)
    {
        var bridge = Marshal.GetDelegateForFunctionPointer<NativeStatusBridge>(nativeBridge);

        try
        {
            NativeCallBoundary.ThrowIfFailed(bridge(Marshal.GetFunctionPointerForDelegate(s_ThrowManagedExceptionCallback)));
        }
        catch (ManagedInteropSmokeException ex)
        {
            return ReferenceEquals(ex, s_LastManagedException)
                && ex.Message == ManagedExceptionMessage
                && ex.Marker == 42
                    ? 1
                    : -1;
        }
        catch (NativeException)
        {
            return -2;
        }
        catch
        {
            return -3;
        }

        return 0;
    }

    public static int MeasureManagedExceptionTokenReleaseAfterNativeSwallow(nint nativeBridge)
    {
        var bridge = Marshal.GetDelegateForFunctionPointer<NativeStatusBridge>(nativeBridge);
        int beforeCount = GetCapturedManagedExceptionCount();

        NativeCallBoundary.ThrowIfFailed(bridge(Marshal.GetFunctionPointerForDelegate(s_ThrowManagedExceptionCallback)));

        int afterCount = GetCapturedManagedExceptionCount();
        return afterCount - beforeCount;
    }

    public static NativeCallStatus RoundTripNativeExceptionThroughManaged(nint nativeCallback)
    {
        s_ObservedNativeExceptionWrapper = 0;
        var callback = Marshal.GetDelegateForFunctionPointer<NativeStatusCallback>(nativeCallback);

        try
        {
            NativeCallBoundary.ThrowIfFailed(callback());
            return ManagedCallBoundary.Succeed();
        }
        catch (NativeException ex)
        {
            s_ObservedNativeExceptionWrapper = IsNativeExceptionWrapper(ex) ? 1 : -1;

            return ManagedCallBoundary.Capture(ex);
        }
        catch (Exception ex)
        {
            s_ObservedNativeExceptionWrapper = -2;
            return ManagedCallBoundary.Capture(ex);
        }
    }

    public static NativeCallStatus ConsumeNativeExceptionThroughManaged(nint nativeCallback)
    {
        s_ObservedNativeExceptionWrapper = 0;

        try
        {
            ConsumeNativeException(nativeCallback);
            CollectFinalizers();
            return ManagedCallBoundary.Succeed();
        }
        catch (Exception ex)
        {
            return ManagedCallBoundary.Capture(ex);
        }
    }

    public static int GetObservedNativeExceptionWrapper()
    {
        return s_ObservedNativeExceptionWrapper;
    }

    private static void ConsumeNativeException(nint nativeCallback)
    {
        var callback = Marshal.GetDelegateForFunctionPointer<NativeStatusCallback>(nativeCallback);

        try
        {
            NativeCallBoundary.ThrowIfFailed(callback());
        }
        catch (NativeException ex)
        {
            s_ObservedNativeExceptionWrapper = IsNativeExceptionWrapper(ex) ? 1 : -1;
        }
    }

    private static bool IsNativeExceptionWrapper(NativeException ex)
    {
        return ex.NativeTypeName.Contains(nameof(InvalidOperationException), StringComparison.Ordinal)
            && ex.Message == NativeExceptionMessage;
    }

    private static void CollectFinalizers()
    {
        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();
    }

    private static int GetCapturedManagedExceptionCount()
    {
        object exceptionsLock = GetManagedExceptionInteropField<object>("m_ManagedExceptionsLock");

        lock (exceptionsLock)
        {
            ICollection exceptions = GetManagedExceptionInteropField<ICollection>("m_ManagedExceptions");
            return exceptions.Count;
        }
    }

    private static T GetManagedExceptionInteropField<T>(string fieldName) where T : class
    {
        FieldInfo field = typeof(ManagedExceptionInterop).GetField(fieldName, BindingFlags.Static | BindingFlags.NonPublic)
            ?? throw new InvalidOperationException($"ManagedExceptionInterop field '{fieldName}' was not found.");

        return field.GetValue(null) as T
            ?? throw new InvalidOperationException($"ManagedExceptionInterop field '{fieldName}' has an unexpected value.");
    }

    private static NativeCallStatus ThrowManagedExceptionCallback()
    {
        try
        {
            s_LastManagedException = new ManagedInteropSmokeException(ManagedExceptionMessage, 42);
            throw s_LastManagedException;
        }
        catch (Exception ex)
        {
            return ManagedCallBoundary.Capture(ex);
        }
    }
}
