// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;

namespace AylaEngine;

public class GenericWindow : IDisposable
{
    private readonly ulong m_InstancePtr;

    internal GenericWindow(ulong instancePtr)
    {
        m_InstancePtr = instancePtr;
    }

    public void Dispose()
    {
        Internal_Dispose(m_InstancePtr);
        GC.SuppressFinalize(this);
    }

    public void Show() => Internal_Show(m_InstancePtr);

    public void Hide() => Internal_Hide(m_InstancePtr);

    [DllImport("ApplicationCore", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint = "GenericWindow_Interop_Dispose")]
    private static extern void Internal_Dispose(ulong instancePtr);

    [DllImport("ApplicationCore", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint = "GenericWindow_Interop_Show")]
    private static extern void Internal_Show(ulong instancePtr);

    [DllImport("ApplicationCore", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint = "GenericWindow_Interop_Show")]
    private static extern void Internal_Hide(ulong instancePtr);
}
