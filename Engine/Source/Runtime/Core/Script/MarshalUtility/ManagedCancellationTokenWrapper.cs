using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
public readonly struct ManagedCancellationTokenWrapper
{
    public readonly nint Handle;
    public readonly nint Ptr;

    public CancellationToken AsManaged()
    {
        if (Handle != 0)
        {
            var cts = (CancellationTokenSource)GCHandle.FromIntPtr(Handle).Target!;
            return cts.Token;
        }
        else
        {
            return CancellationToken.None;
        }
    }
}
