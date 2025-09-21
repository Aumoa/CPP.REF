using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public readonly struct ManagedStringWrapper
{
    public readonly nint C_str;
    public readonly int Length;
    public readonly nint IntRef;

    public unsafe ManagedStringWrapper(char* c_str, int length)
    {
        C_str = (nint)c_str;
        Length = length;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public unsafe string AsManaged()
    {
        try
        {
            return new string((char*)C_str, 0, Length);
        }
        finally
        {
            if (IntRef != 0)
            {
                FreeIntRef__Injected(IntRef);
            }
        }
    }

    [DllImport("Core", EntryPoint = "Ayla__ManagedStringWrapper__FreeIntRef__Injected")]
    private static extern void FreeIntRef__Injected(nint intRef);
}
