// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;

namespace AylaEngine;

public class GenericApplication : IDisposable
{
    private static GenericApplication? s_Instance;

    private readonly ulong m_InstancePtr;

    private GenericApplication(ulong instancePtr)
    {
        m_InstancePtr = instancePtr;
    }

    public GenericWindow MakeWindow()
    {
        var instancePtr = Internal_MakeWindow(m_InstancePtr);
        return new GenericWindow(instancePtr);
    }

    void IDisposable.Dispose()
    {
        Internal_Dispose(m_InstancePtr);
        s_Instance = null;
        GC.SuppressFinalize(this);
    }

    public static GenericApplication CreateApplication()
    {
        if (s_Instance != null)
        {
            throw new InvalidOperationException("GenericApplication instance already created.");
        }

        s_Instance = new GenericApplication(Internal_CreateApplication());
        return s_Instance;
    }

    public static void DestroyApplication()
    {
        if (s_Instance == null)
        {
            throw new InvalidOperationException("GenericApplication instance is not created.");
        }

        ((IDisposable)s_Instance).Dispose();
    }

    public static GenericApplication Instance => s_Instance ?? throw new InvalidOperationException("GenericApplication is not created.");

    [DllImport("ApplicationCore", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint = "GenericApplication_Interop_CreateApplication")]
    private static extern ulong Internal_CreateApplication();

    [DllImport("ApplicationCore", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint = "GenericApplication_Interop_Dispose")]
    private static extern void Internal_Dispose(ulong instancePtr);

    [DllImport("ApplicationCore", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint = "GenericApplication_Interop_MakeWindow")]
    private static extern ulong Internal_MakeWindow(ulong instancePtr);
}
