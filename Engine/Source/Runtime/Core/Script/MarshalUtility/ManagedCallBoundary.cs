namespace Ayla;

public static class ManagedCallBoundary
{
    public static NativeCallStatus Succeed()
    {
        return NativeCallStatus.Success;
    }

    public static NativeCallStatus Capture(global::System.Exception exception)
    {
        return ManagedExceptionInterop.Capture(exception);
    }
}
