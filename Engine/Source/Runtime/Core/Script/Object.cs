using System.Runtime.InteropServices;

namespace Ayla;

public partial class Object : IDisposable
{
    [Flags]
    public enum CreationFlags
    {
        None,
        FromScript = 1 << 0
    }

    protected Object(nint instanceId, CreationFlags flags)
    {
        InstanceId = instanceId;
        if (flags.HasFlag(CreationFlags.FromScript))
        {
            var handle = BeginWriteGCHandle__Injected(InstanceId);
            if (handle != default)
            {
                throw new InvalidOperationException("The instance is already managed by C#.");
            }

            var gch = GCHandle.Alloc(this, GCHandleType.Weak);
            EndWriteGCHandle__Injected(InstanceId, (nint)gch);
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
        var iid = InstanceId;
        InstanceId = 0;
        BeginWriteGCHandle__Injected(iid);
        EndWriteGCHandle__Injected(iid, 0);
    }

    public nint InstanceId { get; private set; }

    public ObjectReferenceWrapper AsWrapper() => AsWrapper__Injected(InstanceId);

    [DllImport("Core", EntryPoint = "Ayla__Object__BeginWriteGCHandle__Injected")]
    internal static extern nint BeginWriteGCHandle__Injected(nint instanceId);

    [DllImport("Core", EntryPoint = "Ayla__Object__EndWriteGCHandle__Injected")]
    internal static extern void EndWriteGCHandle__Injected(nint instanceId, nint handle);

    [DllImport("Core", EntryPoint = "Ayla__Object__AsWrapper__Injected")]
    internal static extern ObjectReferenceWrapper AsWrapper__Injected(nint instanceId);
}
