// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Runtime.InteropServices;
using System.Runtime.Serialization;

namespace AE.CoreAObject;

/// <summary>
/// 관리되는 엔진 인스턴스의 루트 클래스입니다.
/// </summary>
[Serializable]
public class Object : IDisposable
{
    private IntPtr nativePointer;

    /// <summary>
    /// <see cref="Object"/> 클래스의 새 인스턴스를 초기화합니다.
    /// </summary>
    protected Object(IntPtr nativePointer)
    {
        this.nativePointer = nativePointer;
    }

    /// <summary>
    /// <see cref="Object"/> 클래스의 네이티브 인스턴스를 생성합니다.
    /// </summary>
    public Object() : this(InteropType.NativeCall_Construct(NativeCall_StaticClass()))
    {
    }

    /// <inheritdoc/>
    ~Object()
    {
        Dispose(false);
    }

    /// <inheritdoc/>
    public void Dispose()
    {
        Dispose(true);
    }

    /// <summary>
    /// 관리되는 리소스 및 관리되지 않는 리소스를 해제합니다.
    /// </summary>
    /// <param name="disposing"> 개체가 올바른 Dispose 절차에 맞춰 Dispose 되었음을 의미합니다. </param>
    protected virtual void Dispose(bool disposing)
    {
        nativePointer = IntPtr.Zero;
    }

    /// <summary>
    /// 이 오브젝트의 강력 참조 횟수를 가져옵니다.
    /// </summary>
    public int locks { get; }

    /// <summary>
    /// 이 오브젝트의 약한 참조 횟수를 가져옵니다.
    /// </summary>
    public int weaks { get; }

    /// <summary>
    /// 이 오브젝트가 네이티브 수명이 유효한지 나타내는 값을 가져옵니다.
    /// </summary>
    public bool isValid { get; }

#pragma warning disable CS1591

    public static implicit operator bool(Object obj) => obj.isValid;

#pragma warning restore CS1591

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Object_StaticClass", CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr NativeCall_StaticClass();

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Object_GetLocks", CallingConvention = CallingConvention.Cdecl)]
    internal static extern int NativeCall_GetLocks(IntPtr self);

    [DllImport("CoreAObject", EntryPoint = "NativeCall_CoreAObject_Object_GetWeaks", CallingConvention = CallingConvention.Cdecl)]
    internal static extern int NativeCall_GetWeaks(IntPtr self);
}
