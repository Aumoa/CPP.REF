using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
internal unsafe struct CoreCLRFunctions
{
    public void* AsHardHandlePtr;
    public void* AsWeakHandlePtr;
    public void* CreateManagedInstancePtr;
    public void* FreeGCHandlePtr;

    private static CoreCLRFunctions Get__Invoke()
    {
        return new CoreCLRFunctions
        {
            AsHardHandlePtr = (delegate* unmanaged[Cdecl]<nint*, void>)&AsHardHandle__Invoke,
            AsWeakHandlePtr = (delegate* unmanaged[Cdecl]<nint*, void>)&AsWeakHandle__Invoke,
            CreateManagedInstancePtr = (delegate* unmanaged[Cdecl]<nint, ObjectReferenceWrapper>)&CreateManagedInstance__Invoke,
            FreeGCHandlePtr = (delegate* unmanaged[Cdecl]<nint, void>)&FreeGCHandle__Invoke
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

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
    private static ObjectReferenceWrapper CreateManagedInstance__Invoke(nint scriptTypeGetter)
    {
        var scriptType = Marshal.GetDelegateForFunctionPointer<GetScriptTypeDelegate>(scriptTypeGetter)();
        var obj = (Object?)Activator.CreateInstance(scriptType) 
            ?? throw new InvalidOperationException("Failed to create instance: Type does not have a default constructor or is not assignable to Object.");
        return obj.AsWrapper();
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
    private static void FreeGCHandle__Invoke(nint gcHandlePtr)
    {
        var gcHandle = GCHandle.FromIntPtr(gcHandlePtr);
        gcHandle.Free();
    }
}
