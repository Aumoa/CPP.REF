using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public struct ObjectReferenceLocker
{
    public nint Ref;
    public int Flags;

    [DllImport("Core", EntryPoint = "Ayla__ObjectReferenceLocker__GetRawPointer__Injected")]
    internal static extern nint GetRawPointer__Injected(ref ObjectReferenceLocker self);

    [DllImport("Core", EntryPoint = "Ayla__ObjectReferenceLocker__Destroy__Injected")]
    internal static extern void Destroy__Injected(ref ObjectReferenceLocker self);
}
