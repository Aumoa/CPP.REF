using System.Runtime.InteropServices;

namespace Ayla;


public partial class Object : IDisposable, IStaticObject
{
    internal class InternalCreation
    {
        public static ThreadLocal<InternalCreation> ThreadLocal = new(() => new InternalCreation());
    }

    protected Object(Func<object, ObjectReferenceWrapper> locker)
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

        var gcHandlePtr = ClearGCHandle__Injected(nativePointer, m_GCHandleSerial);
        m_GCHandleSerial = 0;
        if (gcHandlePtr != 0)
        {
            GCHandle.FromIntPtr(gcHandlePtr).Free();
        }
    }

    private nint m_NativePointer;
    private ulong m_GCHandleSerial;

    public nint NativePointer => m_NativePointer;

    internal ObjectReferenceWrapper AsWrapper()
    {
        var nativePointer = NativePointer;
        if (nativePointer == 0)
        {
            throw new ObjectDisposedException(GetType().FullName);
        }

        return AsWrapper__Injected(nativePointer) with
        {
            IntGCHandlePtr = (nint)GCHandle.Alloc(this, GCHandleType.Normal)
        };
    }

    public virtual ManagedTypeWrapper GetClass() => StaticClass();

    public static ManagedTypeWrapper StaticClass() => GetManagedType__Injected();

    public static implicit operator bool(Object self) => self != null;

    private static GetScriptTypeDelegate s_GetScriptType__Delegate = () => typeof(Object);

    private static nint GetScriptType__Invoke()
    {
        return Marshal.GetFunctionPointerForDelegate(s_GetScriptType__Delegate);
    }

    [DllImport("Core", EntryPoint = "Ayla__Object__BeginWriteGCHandle__Injected")]
    internal static extern nint BeginWriteGCHandle__Injected(nint instancePtr);

    [DllImport("Core", EntryPoint = "Ayla__Object__EndWriteGCHandle__Injected")]
    internal static extern ulong EndWriteGCHandle__Injected(nint instancePtr, nint handle, bool releaseIntPtr);

    [DllImport("Core", EntryPoint = "Ayla__Object__ClearGCHandle__Injected")]
    internal static extern nint ClearGCHandle__Injected(nint instancePtr, ulong gcHandleSerial);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedType__Injected")]
    internal static extern ManagedTypeWrapper GetManagedType__Injected();

    [DllImport("Core", EntryPoint = "Ayla__Object__AsWrapper__Injected")]
    internal static extern ObjectReferenceWrapper AsWrapper__Injected(nint instancePtr);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedTypeFromPtr__Injected")]
    internal static extern ManagedTypeWrapper GetManagedTypeFromPtr__Injected(nint instancePtr);

    [DllImport("Core", EntryPoint = "Ayla__Object__ReleaseIntermediatePtr__Injected")]
    internal static extern void ReleaseIntermediatePtr__Injected(nint instancePtr);
}
