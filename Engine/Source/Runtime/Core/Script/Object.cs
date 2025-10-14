using System.Runtime.InteropServices;

namespace Ayla;


public partial class Object : IDisposable
{
    internal class InternalCreation
    {
        public static ThreadLocal<InternalCreation> ThreadLocal = new(() => new InternalCreation());

        public bool CreatedByResolver;
    }
        

    private ObjectReferenceLocker m_Locker;

    protected Object(ObjectReferenceLocker locker)
    {
        m_Locker = locker;
        NativePointer = ObjectReferenceLocker.GetRawPointer__Injected(ref locker);

        GCHandle gch = default;
        try
        {
            if (InternalCreation.ThreadLocal.Value!.CreatedByResolver == false)
            {
                var handle = BeginWriteGCHandle__Injected(NativePointer);
                if (handle != default)
                {
                    throw new InvalidOperationException("The instance is already managed by C#.");
                }

                gch = GCHandle.Alloc(this, GCHandleType.Weak);
                handle = (nint)gch;
            }
        }
        finally
        {
            if (InternalCreation.ThreadLocal.Value!.CreatedByResolver)
            {
                InternalCreation.ThreadLocal.Value.CreatedByResolver = false;
            }
            else
            {
                EndWriteGCHandle__Injected(NativePointer, (nint)gch);
            }
        }
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
        BeginWriteGCHandle__Injected(iid);
        EndWriteGCHandle__Injected(iid, 0);
        ObjectReferenceLocker.Destroy__Injected(ref m_Locker);
    }

    public nint NativePointer { get; private set; }

    public ObjectReferenceWrapper AsWrapper() => AsWrapper__Injected(NativePointer);

    public static ManagedTypeWrapper GetManagedType() => GetManagedType__Injected();

    private static GetScriptTypeDelegate s_GetScriptType__Delegate = () => typeof(Object);

    private static nint GetScriptType__Invoke()
    {
        return Marshal.GetFunctionPointerForDelegate(s_GetScriptType__Delegate);
    }

    [DllImport("Core", EntryPoint = "Ayla__Object__DeleteIntermediateRef__Injected")]
    internal static extern void DeleteIntermediateRef__Injected(nint self);

    [DllImport("Core", EntryPoint = "Ayla__Object__BeginWriteGCHandle__Injected")]
    internal static extern nint BeginWriteGCHandle__Injected(nint instanceId);

    [DllImport("Core", EntryPoint = "Ayla__Object__EndWriteGCHandle__Injected")]
    internal static extern void EndWriteGCHandle__Injected(nint instanceId, nint handle);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedType__Injected")]
    internal static extern ManagedTypeWrapper GetManagedType__Injected();

    [DllImport("Core", EntryPoint = "Ayla__Object__AsWrapper__Injected")]
    internal static extern ObjectReferenceWrapper AsWrapper__Injected(nint instanceId);

    [DllImport("Core", EntryPoint = "Ayla__Object__CreateLocker__Injected")]
    internal static extern ObjectReferenceLocker CreateLocker__Injected(nint instanceId);

    [DllImport("Core", EntryPoint = "Ayla__Object__GetManagedTypeFromPtr__Injected")]
    internal static extern ManagedTypeWrapper GetManagedTypeFromPtr__Injected(nint instanceId);
}
