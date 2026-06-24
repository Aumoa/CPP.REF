using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
internal unsafe struct CoreCLRFunctions
{
    public void* m_AsHardHandlePtr;
    public void* m_AsWeakHandlePtr;
    public void* m_CreateManagedInstancePtr;
    public void* m_FreeGCHandlePtr;

    private static NativeCallStatus Get__Invoke(CoreCLRFunctions* functions)
    {
        try
        {
            *functions = new CoreCLRFunctions
            {
                m_AsHardHandlePtr = (delegate* unmanaged[Cdecl]<nint*, NativeCallStatus>)&AsHardHandle__Invoke,
                m_AsWeakHandlePtr = (delegate* unmanaged[Cdecl]<nint*, NativeCallStatus>)&AsWeakHandle__Invoke,
                m_CreateManagedInstancePtr = (delegate* unmanaged[Cdecl]<nint, ObjectReferenceWrapper*, NativeCallStatus>)&CreateManagedInstance__Invoke,
                m_FreeGCHandlePtr = (delegate* unmanaged[Cdecl]<nint, NativeCallStatus>)&FreeGCHandle__Invoke
            };

            return ManagedCallBoundary.Succeed();
        }
        catch (Exception ex)
        {
            return ManagedCallBoundary.Capture(ex);
        }
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
    private static NativeCallStatus AsHardHandle__Invoke(nint* gcHandlePtr)
    {
        try
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

            return ManagedCallBoundary.Succeed();
        }
        catch (Exception ex)
        {
            return ManagedCallBoundary.Capture(ex);
        }
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
    private static NativeCallStatus AsWeakHandle__Invoke(nint* gcHandlePtr)
    {
        try
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

            return ManagedCallBoundary.Succeed();
        }
        catch (Exception ex)
        {
            return ManagedCallBoundary.Capture(ex);
        }
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
    private static NativeCallStatus CreateManagedInstance__Invoke(nint scriptTypeGetter, ObjectReferenceWrapper* wrapper)
    {
        try
        {
            var scriptType = Marshal.GetDelegateForFunctionPointer<GetScriptTypeDelegate>(scriptTypeGetter)();
            var obj = (Object?)Activator.CreateInstance(scriptType)
                ?? throw new InvalidOperationException("Failed to create instance: Type does not have a default constructor or is not assignable to Object.");
            *wrapper = obj.AsWrapper();

            return ManagedCallBoundary.Succeed();
        }
        catch (Exception ex)
        {
            return ManagedCallBoundary.Capture(ex);
        }
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
    private static NativeCallStatus FreeGCHandle__Invoke(nint gcHandlePtr)
    {
        try
        {
            var gcHandle = GCHandle.FromIntPtr(gcHandlePtr);
            gcHandle.Free();

            return ManagedCallBoundary.Succeed();
        }
        catch (Exception ex)
        {
            return ManagedCallBoundary.Capture(ex);
        }
    }
}
