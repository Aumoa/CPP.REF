using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ObjectReferenceWrapper
{
    public nint InstanceId;
    public nint Handle;
    public Object.CreationFlags Flags;

    public ObjectReferenceWrapper(nint instanceId, nint handle, Object.CreationFlags flags)
    {
        InstanceId = instanceId;
        Handle = handle;
        Flags = flags;
    }

    public T? As<T>() where T : Object
    {
        if (InstanceId == 0)
        {
            return null;
        }

        Object.BeginWriteGCHandle__Injected(InstanceId);
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

            var inst = (T?)Activator.CreateInstance(typeof(T), BindingFlags.NonPublic | BindingFlags.Instance, null, [InstanceId], null);
            return inst;
        }
        finally
        {
            Object.EndWriteGCHandle__Injected(InstanceId, (nint)handle);
        }
    }
}
