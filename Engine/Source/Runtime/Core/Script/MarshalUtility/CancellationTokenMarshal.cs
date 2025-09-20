using System.Runtime.InteropServices;

namespace Ayla;

internal static class CancellationTokenMarshal
{
    public static void Cancel(nint cancellationTokenSource)
    {
        var gchandle = GCHandle.FromIntPtr(cancellationTokenSource);
        ((CancellationTokenSource)gchandle.Target!).Cancel();
    }

    public static nint CreateCancellationTokenSource()
    {
        var cts = new CancellationTokenSource();
        var gchandle = GCHandle.Alloc(cts);
        return GCHandle.ToIntPtr(gchandle);
    }

    public static void FreeCancellationTokenSource(nint cts)
    {
        GCHandle.FromIntPtr(cts).Free();
    }
}
