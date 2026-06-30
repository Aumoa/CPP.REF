using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct NativeObjectReferenceWrapper
{
    public nint Ptr;
    public nint IntGCHandlePtr;
    public ulong GCHandleSerial;

    public T? AsManaged<T>() where T : Object => ObjectReferenceMarshaller.AsManaged<T>(Ptr);
}

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ManagedObjectReferenceWrapper
{
    public nint Ptr;
    public nint IntGCHandlePtr;
    public ulong GCHandleSerial;

    public static implicit operator ManagedObjectReferenceWrapper(Object? obj) => obj?.AsManagedObjectReferenceWrapper() ?? default;
}

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct BoundObjectReferenceWrapper
{
    public nint Ptr;
    public ulong GCHandleSerial;
}

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ObjectReferenceWrapper
{
    public nint Ptr;
    public nint IntGCHandlePtr;
    public ulong GCHandleSerial;

    public T? AsManaged<T>() where T : Object => ObjectReferenceMarshaller.AsManaged<T>(Ptr);

    public static implicit operator ObjectReferenceWrapper(Object? obj) => obj?.AsWrapper() ?? default;
}

internal static class ObjectReferenceMarshaller
{
    public static T? AsManaged<T>(nint ptr) where T : Object
    {
        if (ptr == 0)
        {
            return null;
        }

        var managedType = Object.GetManagedTypeFromPtr(ptr);
        var scriptType = managedType.GetScriptType();

        var handlePtr = Object.BeginWriteGCHandle(ptr);
        var writeLocked = true;
        var writeCompleted = false;
        var staleHandlePtr = nint.Zero;
        try
        {
            if (!typeof(T).IsAssignableFrom(scriptType))
            {
                EndWrite(handlePtr, true);
                throw new InvalidCastException($"Cannot convert managed wrapper type '{scriptType.FullName}' to '{typeof(T).FullName}'.");
            }

            var existing = TryGetExistingManagedObject(handlePtr);
            if (existing != null)
            {
                return existing;
            }

            if (handlePtr != 0)
            {
                EndWrite(handlePtr, false);
                handlePtr = 0;
                GC.WaitForPendingFinalizers();

                handlePtr = Object.BeginWriteGCHandle(ptr);
                writeLocked = true;
                existing = TryGetExistingManagedObject(handlePtr);
                if (existing != null)
                {
                    return existing;
                }

                if (handlePtr != 0)
                {
                    staleHandlePtr = handlePtr;
                }
            }

            Func<object, BoundObjectReferenceWrapper> locker = @this =>
            {
                var newHandlePtr = (nint)GCHandle.Alloc(@this, GCHandleType.Normal);
                try
                {
                    var gcHandleSerial = Object.EndWriteGCHandleAndGetSerial(ptr, newHandlePtr, true);
                    writeLocked = false;
                    writeCompleted = true;
                    if (staleHandlePtr != 0)
                    {
                        GCHandle.FromIntPtr(staleHandlePtr).Free();
                    }

                    return new BoundObjectReferenceWrapper
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
            if (!writeCompleted && writeLocked)
            {
                Object.EndWriteGCHandle(ptr, handlePtr, true);
            }

            throw;
        }

        T? TryGetExistingManagedObject(nint existingHandlePtr)
        {
            if (existingHandlePtr == 0)
            {
                return null;
            }

            var target = GCHandle.FromIntPtr(existingHandlePtr).Target;
            if (target == null)
            {
                return null;
            }

            EndWrite(existingHandlePtr, true);
            if (target is T t)
            {
                return t;
            }

            throw new InvalidCastException($"Cannot convert existing managed wrapper type '{target.GetType().FullName}' to '{typeof(T).FullName}'.");
        }

        void EndWrite(nint nextHandlePtr, bool releaseIntPtr)
        {
            Object.EndWriteGCHandle(ptr, nextHandlePtr, releaseIntPtr);
            writeLocked = false;
            writeCompleted = releaseIntPtr;
        }
    }
}
