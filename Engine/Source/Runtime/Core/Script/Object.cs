using System.Runtime.InteropServices;

namespace Ayla;

public partial class Object
{
    protected Object(nint instanceId)
    {
        InstanceId = instanceId;
    }

    public nint InstanceId { get; }
}
