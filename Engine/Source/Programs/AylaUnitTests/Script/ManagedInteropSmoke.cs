// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;

namespace Ayla.Tests;

public static class ManagedInteropSmoke
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate int IntBinaryOperation(int left, int right);

    private static int s_Counter;

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
}
