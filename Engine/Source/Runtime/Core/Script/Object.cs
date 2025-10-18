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
        var gcHandlePtr = BeginWriteGCHandle__Injected(iid);
        EndWriteGCHandle__Injected(iid, 0, false);
        if (gcHandlePtr == 0)
        {
            throw new InvalidOperationException();
        }

        GCHandle.FromIntPtr(gcHandlePtr).Free();
    }

    public nint NativePointer { get; private set; }

    public ObjectReferenceWrapper AsWrapper() => AsWrapper__Injected(NativePointer);

    public virtual ManagedTypeWrapper GetClass() => StaticClass();

    public static ManagedTypeWrapper StaticClass() => GetManagedType__Injected();

    private static GetScriptTypeDelegate s_GetScriptType__Delegate = () => typeof(Object);

    private static nint GetScriptType__Invoke()
    {
        return Marshal.GetFunctionPointerForDelegate(s_GetScriptType__Delegate);
    }

    [DllImport("Core", EntryPoint = "Ayla__Object__BeginWriteGCHandle__Injected")]
    internal static extern nint BeginWriteGCHandle__Injected(nint instancePtr);

    [DllImport("Core", EntryPoint = "Ayla__Object__EndWriteGCHandle__Injected")]
    internal static extern void EndWriteGCHandle__Injected(nint instancePtr, nint handle, bool releaseIntPtr);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedType__Injected")]
    internal static extern ManagedTypeWrapper GetManagedType__Injected();

    [DllImport("Core", EntryPoint = "Ayla__Object__AsWrapper__Injected")]
    internal static extern ObjectReferenceWrapper AsWrapper__Injected(nint instancePtr);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedTypeFromPtr__Injected")]
    internal static extern ManagedTypeWrapper GetManagedTypeFromPtr__Injected(nint instancePtr);

    [DllImport("Core", EntryPoint = "Ayla__Object__ReleaseIntermediatePtr__Injected")]
    internal static extern void ReleaseIntermediatePtr__Injected(nint instancePtr);
}
