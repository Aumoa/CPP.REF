namespace Ayla;

public static class ManagedCallBoundary
{
    public static NativeCallStatus Succeed()
    {
        return NativeCallStatus.Succeeded;
    }

    public static NativeCallStatus Capture(global::System.Exception exception)
    {
        return ManagedExceptionInterop.Capture(exception);
    }
}
