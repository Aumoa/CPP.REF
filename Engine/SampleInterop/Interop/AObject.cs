﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.
// Generated by AylaHeaderTool.
// Do NOT modify this code manually!

using System.Diagnostics;
using System.Runtime.InteropServices;

namespace CoreAObject;

public class AObject : IDisposable
{
    public nint NativeHandle { get; private set; }

    public AObject()
    {
        NativeHandle = CoreAObject__AObject__Construct();
    }

    ~AObject()
    {
        Dispose(false);
    }

    public void Dispose()
    {
        Dispose(true);
    }

    protected virtual void Dispose(bool bDisposing)
    {
        if (bDisposing)
        {
            CoreAObject__AObject__Destruct(NativeHandle);
            NativeHandle = 0;
        }

        Debug.Assert(NativeHandle == 0);
        GC.SuppressFinalize(this);
    }

    public override string ToString()
    {
        nint Cstr = CoreAObject__AObject__ToString(NativeHandle);
        return Marshal.PtrToStringUni(Cstr) ?? string.Empty;
    }

    [DllImport("CoreAObject.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern nint CoreAObject__AObject__Construct();

    [DllImport("CoreAObject.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern void CoreAObject__AObject__Destruct(nint NativeHandle);

    [DllImport("CoreAObject.dll", CallingConvention = CallingConvention.Cdecl)]
    private static extern nint CoreAObject__AObject__ToString(nint NativeHandle);
}
