using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public readonly unsafe struct ManagedTaskWrapper
{
    public readonly void* TaskRef;
}
