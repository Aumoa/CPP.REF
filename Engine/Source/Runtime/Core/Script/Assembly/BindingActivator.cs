// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Runtime.InteropServices;

namespace Ayla;

public static class BindingActivator
{
    private static object[]? s_Hold;

    public static void Register()
    {
        CreateManagedInstanceDelegate createManagedInstance = CreateManagedInstance;
        RegisterBindingCallbacks(createManagedInstance);
        s_Hold = [createManagedInstance];
    }

    private static nint CreateManagedInstance(string bindingTypeName)
    {
        if (ReflectionUtility.FindTypeSimple(bindingTypeName, out var type) != ReflectionUtility.FindResult.Success)
        {
            return 0;
        }

        var instance = (Object?)Activator.CreateInstance(type!, true);
        if (instance == null)
        {
            return 0;
        }

        return (nint)instance.InstancePtr;
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    private delegate nint CreateManagedInstanceDelegate(string bindingTypeName);

    [DllImport("Core", EntryPoint = "Ayla__RegisterBindingCallbacks")]
    private static extern nint RegisterBindingCallbacks(CreateManagedInstanceDelegate createManagedInstanceCallback);
}
