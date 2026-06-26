using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ObjectReferenceWrapper
{
    public nint Ptr;
    public nint IntGCHandlePtr;

    public T? AsManaged<T>() where T : Object
    {
        if (Ptr == 0)
        {
            return null;
        }

        var managedType = Object.GetManagedTypeFromPtr(Ptr);
        var scriptType = managedType.GetScriptType();

        nint handlePtr = Object.BeginWriteGCHandle(Ptr);
        GCHandle handle = default;
        try
        {
            if (handlePtr != 0)
            {
                handle = GCHandle.FromIntPtr(handlePtr);
                if (handle.Target is T t)
                {
                    Object.EndWriteGCHandle(Ptr, handlePtr, true);
                    return t;
                }
            }

            var ptr = Ptr;
            Func<object, nint> locker = @this =>
            {
                Object.EndWriteGCHandle(ptr, (nint)GCHandle.Alloc(@this, GCHandleType.Normal), true);
                return ptr;
            };

            return (T?)Activator.CreateInstance(scriptType, BindingFlags.NonPublic | BindingFlags.Instance, null, [locker], null);
        }
        catch
        {
            Object.EndWriteGCHandle(Ptr, 0, true);
            throw;
        }
    }

    public static implicit operator ObjectReferenceWrapper(Object? obj) => obj?.AsWrapper() ?? default;
}
