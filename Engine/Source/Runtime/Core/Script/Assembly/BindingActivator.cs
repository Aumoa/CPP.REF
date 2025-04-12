// Copyright 2020-2025 AylaEngine. All Rights Reserved.

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

    private static void CreateManagedInstance(int instanceId, string bindingTypeName)
    {
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    private delegate void CreateManagedInstanceDelegate(int instanceId, string bindingTypeName);

    [DllImport("Core", EntryPoint = "Ayla__RegisterBindingCallbacks")]
    private static extern void RegisterBindingCallbacks(CreateManagedInstanceDelegate createManagedInstanceCallback);
}
