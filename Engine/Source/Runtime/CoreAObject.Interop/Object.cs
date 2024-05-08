using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace AylaEngine;

public class Object : IDisposable
{
    private ulong m_InstancePtr;
    private ulong m_InstanceId;

    protected Object()
    {
        m_InstancePtr = Internals.s_InstancePtr;
        if (m_InstancePtr == 0)
        {
            Debug.LogErrorFormat("Object pointer is not ready.");
            return;
        }
        m_InstanceId = Internal_GetInstanceID(m_InstancePtr);
    }

    ~Object()
    {
        Dispose(false);
    }

    public ulong InstanceID
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_InstanceId;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override bool Equals(object? rhs)
    {
        if (rhs is Object aylaObject)
        {
            return this == aylaObject;
        }

        return false;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override int GetHashCode()
    {
        return HashCode.Combine(m_InstancePtr, m_InstanceId);
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public bool IsValid()
    {
        return m_InstancePtr != 0 && m_InstanceId != 0 && Internal_IsValidInstanceId(m_InstanceId);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    protected virtual void Dispose(bool disposing)
    {
        if (disposing)
        {
            Destroy(this);
        }
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool IsValid(Object obj)
    {
        return obj?.IsValid() == true;
    }

    public static void Destroy(Object obj)
    {
        if (IsValid(obj) == false)
        {
            throw new ArgumentException("'obj' is already destroyed or null instance.");
        }

        Internal_Destroy(obj.m_InstancePtr);
        obj.m_InstancePtr = 0;
        obj.m_InstanceId = 0;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator bool(Object obj)
    {
        return IsValid(obj);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator ==(Object? lhs, Object? rhs)
    {
        ulong leftId = 0;
        ulong rightId = 0;
        if (lhs is not null)
        {
            leftId = lhs.m_InstanceId;
        }
        if (rhs is not null)
        {
            rightId = rhs.m_InstanceId;
        }
        return leftId == rightId;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator !=(Object? lhs, Object? rhs)
    {
        return !(lhs == rhs);
    }

    [DllImport("CoreAObject", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    private static extern bool Internal_IsValidInstanceId(ulong instanceID);

    [DllImport("CoreAObject", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    private static extern void Internal_Destroy(ulong instancePtr);

    [DllImport("CoreAObject", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    private static extern ulong Internal_GetInstanceID(ulong instancePtr);
}
