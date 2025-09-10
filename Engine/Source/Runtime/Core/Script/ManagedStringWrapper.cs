using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ManagedStringWrapper
{
    public nint C_str;
    public int Length;
}
