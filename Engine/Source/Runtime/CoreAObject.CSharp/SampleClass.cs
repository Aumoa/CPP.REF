// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;

namespace AE.CoreAObject;

public class SampleClass : Object
{
    public SampleClass() : base(NativeCall_Construct())
    {
    }

    public int value
    {
        get => NativeCall_Get_value(handle);
        set => NativeCall_Set_value(handle, value);
    }

    public void Assign(SampleClass arg0) => NativeCall_Assign(handle, arg0.handle);

    public override string ToString() => NativeCall_ToString(handle);

    [DllImport("CoreAObject.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "NativeCall_CoreAObject_SampleClass_Construct", CharSet = CharSet.Unicode)]
    private static extern IntPtr NativeCall_Construct();

    [DllImport("CoreAObject.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "NativeCall_CoreAObject_SampleClass_StaticClass", CharSet = CharSet.Unicode)]
    private new static extern IntPtr NativeCall_StaticClass();

    [DllImport("CoreAObject.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "NativeCall_CoreAObject_SampleClass_Assign", CharSet = CharSet.Unicode)]
    private static extern void NativeCall_Assign(IntPtr __this, IntPtr __arg0);

    [DllImport("CoreAObject.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "NativeCall_CoreAObject_SampleClass_ToString", CharSet = CharSet.Unicode)]
    private static extern string NativeCall_ToString(IntPtr __this);

    [DllImport("CoreAObject.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "NativeCall_CoreAObject_SampleClass_Get_value", CharSet = CharSet.Unicode)]
    private static extern int NativeCall_Get_value(IntPtr __this);

    [DllImport("CoreAObject.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "NativeCall_CoreAObject_SampleClass_Set_value", CharSet = CharSet.Unicode)]
    private static extern void NativeCall_Set_value(IntPtr __this, int value);
}
