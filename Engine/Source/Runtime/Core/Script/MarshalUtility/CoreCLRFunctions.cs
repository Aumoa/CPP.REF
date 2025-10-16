using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
internal unsafe struct CoreCLRFunctions
{
    public void* AsHardHandlePtr;
    public void* AsWeakHandlePtr;

    private static CoreCLRFunctions Get__Invoke()
    {
        return new CoreCLRFunctions
        {
            AsHardHandlePtr = (delegate* unmanaged[Cdecl]<nint*, void>)&AsHardHandle__Invoke,
            AsWeakHandlePtr = (delegate* unmanaged[Cdecl]<nint*, void>)&AsWeakHandle__Invoke
        };
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
    private static void AsHardHandle__Invoke(nint* gcHandlePtr)
    {
        var gcHandle = GCHandle.FromIntPtr(*gcHandlePtr);
        var target = gcHandle.Target;
        if (target == null)
        {
            throw new InvalidOperationException();
        }

        gcHandle.Free();
        gcHandle = GCHandle.Alloc(target, GCHandleType.Normal);
        *gcHandlePtr = (nint)gcHandle;
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
    private static void AsWeakHandle__Invoke(nint* gcHandlePtr)
    {
        var gcHandle = GCHandle.FromIntPtr(*gcHandlePtr);
        var target = gcHandle.Target;
        if (target == null)
        {
            throw new InvalidOperationException();
        }

        gcHandle.Free();
        gcHandle = GCHandle.Alloc(target, GCHandleType.Weak);
        *gcHandlePtr = (nint)gcHandle;
    }
}
