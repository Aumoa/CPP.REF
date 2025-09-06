using System.Runtime.InteropServices;

namespace Ayla;

public partial class Object
{
    [DllImport("Core", EntryPoint = "Ayla__Object__GetInstanceId_Injected")]
    private static extern nint GetInstanceId(nint self);
}
