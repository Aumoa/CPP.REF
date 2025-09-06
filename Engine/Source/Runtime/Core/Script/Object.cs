using System.Runtime.InteropServices;

namespace Ayla;

public partial class Object
{
    protected Object(nint instanceId)
    {
        InstanceId = instanceId;
    }

    public nint InstanceId { get; }

    [DllImport("Core", EntryPoint = "Ayla__Object__BeginWriteGCHandle")]
    internal static extern void BeginWriteGCHandle(nint instanceId);

    [DllImport("Core", EntryPoint = "Ayla__Object__EndWriteGCHandle")]
    internal static extern void EndWriteGCHandle(nint instanceId);
}
