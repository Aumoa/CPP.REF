// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace Ayla;

public class Object : IDisposable
{
    internal nint m_InstancePtr;

    public Object()
    {
        m_InstancePtr = Injected__Ayla__Object__New();
    }

    protected Object(nint instancePtr)
    {
        m_InstancePtr = instancePtr;
    }

    ~Object()
    {
        Dispose(false);
    }

    public void Dispose()
    {
        Dispose(true);
        System.GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        Injected__Ayla__Object__Finalize(m_InstancePtr);
        m_InstancePtr = 0;
    }

    [DllImport("Core")]
    private static extern nint Injected__Ayla__Object__New();

    [DllImport("Core")]
    private static extern void Injected__Ayla__Object__Finalize(nint self_);

    public static void RegisterWeakReferenceHandle(nint instancePtr, nint gcHandle)
    {
        Injected__Ayla__Object__RegisterWeakReferenceHandle(instancePtr, gcHandle);
    }

    [DllImport("Core")]
    private static extern void Injected__Ayla__Object__RegisterWeakReferenceHandle(nint instancePtr, nint gcHandle);
}
