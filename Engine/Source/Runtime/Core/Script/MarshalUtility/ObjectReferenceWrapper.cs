using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ObjectReferenceWrapper
{
    public nint Ptr;
    public nint Handle;

    public ObjectReferenceWrapper(nint ptr, nint handle)
    {
        Ptr = ptr;
        Handle = handle;
    }

    public T? As<T>() where T : Object
    {
        if (Ptr == 0)
        {
            return null;
        }

        Object.BeginWriteGCHandle__Injected(Ptr);
        GCHandle handle = default;
        try
        {
            if (Handle != 0)
            {
                handle = GCHandle.FromIntPtr(Handle);
                if (handle.Target is T t)
                {
                    return t;
                }
            }

            var locker = Object.CreateLocker__Injected(Ptr);
            Object.InternalCreation.ThreadLocal.Value!.CreatedByResolver = true;
            var inst = (T?)Activator.CreateInstance(typeof(T), BindingFlags.NonPublic | BindingFlags.Instance, null, [locker], null);
            handle = GCHandle.Alloc(inst, GCHandleType.Weak);
            return inst;
        }
        finally
        {
            Object.EndWriteGCHandle__Injected(Ptr, (nint)handle);
        }
    }

    public static implicit operator ObjectReferenceWrapper(Object obj) => obj.AsWrapper();
}
