using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ObjectReferenceWrapper
{
    public nint InstanceId;
    public nint Handle;

    public T? As<T>() where T : Object
    {
        if (InstanceId == 0)
        {
            return null;
        }

        if (Handle == 0)
        {
            return (T?)Activator.CreateInstance(typeof(T), BindingFlags.NonPublic | BindingFlags.Instance, null, [InstanceId], null);
        }
        else
        {
            var handle = GCHandle.FromIntPtr(Handle);
            return handle.Target as T;
        }
    }
}
