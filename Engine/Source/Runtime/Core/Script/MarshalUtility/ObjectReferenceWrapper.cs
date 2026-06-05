using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ObjectReferenceWrapper
{
    public nint Ptr;
    public nint IntGCHandlePtr;
    public ulong GCHandleSerial;

    public T? AsManaged<T>() where T : Object
    {
        if (Ptr == 0)
        {
            return null;
        }

        var managedType = Object.GetManagedTypeFromPtr__Injected(Ptr);
        var scriptType = managedType.GetScriptType();

        var handlePtr = Object.BeginWriteGCHandle__Injected(Ptr);
        var writeCompleted = false;
        try
        {
            if (!typeof(T).IsAssignableFrom(scriptType))
            {
                Object.EndWriteGCHandle__Injected(Ptr, handlePtr, true);
                writeCompleted = true;
                throw new InvalidCastException($"Cannot convert managed wrapper type '{scriptType.FullName}' to '{typeof(T).FullName}'.");
            }

            if (handlePtr != 0)
            {
                var target = GCHandle.FromIntPtr(handlePtr).Target;
                if (target != null)
                {
                    Object.EndWriteGCHandle__Injected(Ptr, handlePtr, true);
                    writeCompleted = true;
                    if (target is T t)
                    {
                        return t;
                    }

                    throw new InvalidCastException($"Cannot convert existing managed wrapper type '{target.GetType().FullName}' to '{typeof(T).FullName}'.");
                }
            }

            var ptr = Ptr;
            Func<object, ObjectReferenceWrapper> locker = @this =>
            {
                var newHandlePtr = (nint)GCHandle.Alloc(@this, GCHandleType.Normal);
                try
                {
                    var gcHandleSerial = Object.EndWriteGCHandle__Injected(ptr, newHandlePtr, true);
                    writeCompleted = true;
                    return new ObjectReferenceWrapper
                    {
                        Ptr = ptr,
                        GCHandleSerial = gcHandleSerial
                    };
                }
                catch
                {
                    GCHandle.FromIntPtr(newHandlePtr).Free();
                    throw;
                }
            };

            return (T?)Activator.CreateInstance(scriptType, BindingFlags.NonPublic | BindingFlags.Instance, null, [locker], null);
        }
        catch
        {
            if (!writeCompleted)
            {
                Object.EndWriteGCHandle__Injected(Ptr, 0, true);
            }

            throw;
        }
    }

    public static implicit operator ObjectReferenceWrapper(Object? obj) => obj?.AsWrapper() ?? default;
}
