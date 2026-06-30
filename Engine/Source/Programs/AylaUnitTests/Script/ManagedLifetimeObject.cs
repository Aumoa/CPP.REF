// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;

namespace Ayla.Tests;

public sealed class ManagedLifetimeObject : Object
{
    private static readonly GetScriptTypeDelegate s_GetScriptType__Delegate = () => typeof(ManagedLifetimeObject);

    private ManagedLifetimeObject(Func<object, BoundObjectReferenceWrapper> locker)
        : base(locker)
    {
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
