using System.Runtime.InteropServices;

namespace Ayla;


public partial class Object : IDisposable, IStaticObject
{
    internal class InternalCreation
    {
        public static ThreadLocal<InternalCreation> ThreadLocal = new(() => new InternalCreation());
    }

    protected Object(Func<object, nint> locker)
    {
        NativePointer = locker(this);
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
        var iid = NativePointer;
        NativePointer = 0;
        var gcHandlePtr = BeginWriteGCHandle(iid);
        EndWriteGCHandle(iid, 0, false);
        if (gcHandlePtr == 0)
        {
            throw new InvalidOperationException();
        }

        GCHandle.FromIntPtr(gcHandlePtr).Free();
    }

    public nint NativePointer { get; private set; }

    internal ObjectReferenceWrapper AsWrapper() => AsWrapper(NativePointer) with
    {
        IntGCHandlePtr = (nint)GCHandle.Alloc(this, GCHandleType.Normal)
    };

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
        NativeCallBoundary.ThrowIfFailed(EndWriteGCHandle__Injected(instancePtr, handle, releaseIntPtr));
    }

    internal static ManagedTypeWrapper GetManagedType()
    {
        NativeCallBoundary.ThrowIfFailed(GetManagedType__Injected(out ManagedTypeWrapper result));
        return result;
    }

    internal static ObjectReferenceWrapper AsWrapper(nint instancePtr)
    {
        NativeCallBoundary.ThrowIfFailed(AsWrapper__Injected(instancePtr, out ObjectReferenceWrapper result));
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
    private static extern NativeCallStatus EndWriteGCHandle__Injected(nint instancePtr, nint handle, [MarshalAs(UnmanagedType.I1)] bool releaseIntPtr);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedType__Injected")]
    private static extern NativeCallStatus GetManagedType__Injected(out ManagedTypeWrapper result);

    [DllImport("Core", EntryPoint = "Ayla__Object__AsWrapper__Injected")]
    private static extern NativeCallStatus AsWrapper__Injected(nint instancePtr, out ObjectReferenceWrapper result);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedTypeFromPtr__Injected")]
    private static extern NativeCallStatus GetManagedTypeFromPtr__Injected(nint instancePtr, out ManagedTypeWrapper result);

}
