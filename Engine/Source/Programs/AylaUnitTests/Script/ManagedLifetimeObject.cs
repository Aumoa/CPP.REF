// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;

namespace Ayla.Tests;

public sealed class ManagedLifetimeObject : Object
{
    private static readonly GetScriptTypeDelegate s_GetScriptType__Delegate = () => typeof(ManagedLifetimeObject);
    private static bool s_ThrowAfterBinding;

    private ManagedLifetimeObject(Func<object, BoundObjectReferenceWrapper> locker)
        : base(locker)
    {
        if (s_ThrowAfterBinding)
        {
            throw new InvalidOperationException("Managed lifetime construction failed after native binding.");
        }
    }

    public static void SetThrowAfterBinding(bool value)
    {
        s_ThrowAfterBinding = value;
    }

    private static unsafe NativeCallStatus GetScriptType__Invoke(nint* result)
    {
        try
        {
            *result = Marshal.GetFunctionPointerForDelegate(s_GetScriptType__Delegate);
            return ManagedCallBoundary.Succeed();
        }
        catch (Exception ex)
        {
            return ManagedCallBoundary.Capture(ex);
        }
    }
}
