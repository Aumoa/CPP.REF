using System.Runtime.InteropServices;

namespace Ayla;


public partial class Object : IDisposable, IStaticObject
{
    internal class InternalCreation
    {
        public static ThreadLocal<InternalCreation> ThreadLocal = new(() => new InternalCreation());
    }

    protected Object(Func<object, BoundObjectReferenceWrapper> locker)
    {
        var wrapper = locker(this);
        m_NativePointer = wrapper.Ptr;
        m_GCHandleSerial = wrapper.GCHandleSerial;
    }

    ~Object()
    {
        Dispose(false);
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        var nativePointer = Interlocked.Exchange(ref m_NativePointer, 0);
        if (nativePointer == 0)
        {
            return;
        }

        var gcHandlePtr = ClearGCHandle(nativePointer, m_GCHandleSerial);
        m_GCHandleSerial = 0;
        if (gcHandlePtr != 0)
        {
            GCHandle.FromIntPtr(gcHandlePtr).Free();
        }
    }

    private nint m_NativePointer;
    private ulong m_GCHandleSerial;

    public nint NativePointer => m_NativePointer;

    internal ManagedObjectReferenceWrapper AsManagedObjectReferenceWrapper()
    {
        var nativePointer = NativePointer;
        if (nativePointer == 0)
        {
            throw new ObjectDisposedException(GetType().FullName);
        }

        return new ManagedObjectReferenceWrapper
        {
            Ptr = nativePointer,
            IntGCHandlePtr = (nint)GCHandle.Alloc(this, GCHandleType.Normal),
            GCHandleSerial = m_GCHandleSerial
        };
    }

    public virtual ManagedTypeWrapper GetClass() => StaticClass();

    public static ManagedTypeWrapper StaticClass() => GetManagedType();

    public static implicit operator bool(Object self) => self != null;

    private static GetScriptTypeDelegate s_GetScriptType__Delegate = () => typeof(Object);

    private static unsafe NativeCallStatus GetScriptType__Invoke(nint* result)
    {
        try
        {
            *result = Marshal.GetFunctionPointerForDelegate(s_GetScriptType__Delegate);
            return ManagedCallBoundary.Succeed();
        }
        catch (Exception ex)
        {
            return ManagedCallBoundary.Capture(ex);
        }
    }

    internal static nint BeginWriteGCHandle(nint instancePtr)
    {
        NativeCallBoundary.ThrowIfFailed(BeginWriteGCHandle__Injected(instancePtr, out nint handle));
        return handle;
    }

    internal static void EndWriteGCHandle(nint instancePtr, nint handle, bool releaseIntPtr)
    {
        _ = EndWriteGCHandleAndGetSerial(instancePtr, handle, releaseIntPtr);
    }

    internal static ulong EndWriteGCHandleAndGetSerial(nint instancePtr, nint handle, bool releaseIntPtr)
    {
        NativeCallBoundary.ThrowIfFailed(EndWriteGCHandle__Injected(instancePtr, handle, releaseIntPtr, out ulong gcHandleSerial));
        return gcHandleSerial;
    }

    internal static nint ClearGCHandle(nint instancePtr, ulong gcHandleSerial)
    {
        NativeCallBoundary.ThrowIfFailed(ClearGCHandle__Injected(instancePtr, gcHandleSerial, out nint handle));
        return handle;
    }

    internal static ManagedTypeWrapper GetManagedType()
    {
        NativeCallBoundary.ThrowIfFailed(GetManagedType__Injected(out ManagedTypeWrapper result));
        return result;
    }

    internal static ManagedTypeWrapper GetManagedTypeFromPtr(nint instancePtr)
    {
        NativeCallBoundary.ThrowIfFailed(GetManagedTypeFromPtr__Injected(instancePtr, out ManagedTypeWrapper result));
        return result;
    }

    [DllImport("Core", EntryPoint = "Ayla__Object__BeginWriteGCHandle__Injected")]
    private static extern NativeCallStatus BeginWriteGCHandle__Injected(nint instancePtr, out nint handle);

    [DllImport("Core", EntryPoint = "Ayla__Object__EndWriteGCHandle__Injected")]
    private static extern NativeCallStatus EndWriteGCHandle__Injected(nint instancePtr, nint handle, [MarshalAs(UnmanagedType.I1)] bool releaseIntPtr, out ulong gcHandleSerial);

    [DllImport("Core", EntryPoint = "Ayla__Object__ClearGCHandle__Injected")]
    private static extern NativeCallStatus ClearGCHandle__Injected(nint instancePtr, ulong gcHandleSerial, out nint handle);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedType__Injected")]
    private static extern NativeCallStatus GetManagedType__Injected(out ManagedTypeWrapper result);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedTypeFromPtr__Injected")]
    private static extern NativeCallStatus GetManagedTypeFromPtr__Injected(nint instancePtr, out ManagedTypeWrapper result);

}
