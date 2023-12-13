// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;
using System.Runtime.Serialization;

namespace AE.CoreAObject;

[Serializable]
public class Object : IDisposable
{
    protected Object(IntPtr nativePointer)
    {
        handle = nativePointer;
    }

    ~Object()
    {
        Dispose(false);
    }

    public void Dispose()
    {
        Dispose(true);
    }

    protected virtual void Dispose(bool disposing)
    {
        if (handle != IntPtr.Zero)
        {
            NativeCall_Release(handle);
            handle = IntPtr.Zero;
        }
    }

    public int locks { get; }

    public int weaks { get; }

    public bool isValid => locks > 0;

    protected IntPtr handle { get; private set; }

    public static implicit operator bool(Object obj) => obj.isValid;

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Object_StaticClass", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    internal static extern IntPtr NativeCall_StaticClass();

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Object_GetLocks", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    internal static extern int NativeCall_GetLocks(IntPtr self);

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Object_GetWeaks", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    internal static extern int NativeCall_GetWeaks(IntPtr self);

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Object_AddRef", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    internal static extern int NativeCall_AddRef(IntPtr self);

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Object_Release", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    internal static extern int NativeCall_Release(IntPtr self);
}
