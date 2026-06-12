// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;

namespace Ayla.Tests;

public sealed class ManagedLifetimeObject : Object
{
    private static readonly GetScriptTypeDelegate s_GetScriptType__Delegate = () => typeof(ManagedLifetimeObject);

    private ManagedLifetimeObject(Func<object, nint> locker)
        : base(locker)
    {
    }

    private static nint GetScriptType__Invoke()
    {
        return Marshal.GetFunctionPointerForDelegate(s_GetScriptType__Delegate);
    }
}
