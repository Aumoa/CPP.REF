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

    protected Object(nint instanceId)
    {
        InstanceId = instanceId;
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
        BeginWriteGCHandle(iid);
        EndWriteGCHandle(iid, 0);
    }

    public nint InstanceId { get; private set; }

    [DllImport("Core", EntryPoint = "Ayla__Object__BeginWriteGCHandle")]
    internal static extern void BeginWriteGCHandle(nint instanceId);

    [DllImport("Core", EntryPoint = "Ayla__Object__EndWriteGCHandle")]
    internal static extern void EndWriteGCHandle(nint instanceId, nint handle);
}
