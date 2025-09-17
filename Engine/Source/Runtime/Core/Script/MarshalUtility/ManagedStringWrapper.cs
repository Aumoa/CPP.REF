using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ManagedStringWrapper
{
    public nint C_str;
    public int Length;

    public unsafe ManagedStringWrapper(char* c_str, int length)
    {
        C_str = (nint)c_str;
        Length = length;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public unsafe string AsManaged()
    {
        return new string((char*)C_str, 0, Length);
    }
}
