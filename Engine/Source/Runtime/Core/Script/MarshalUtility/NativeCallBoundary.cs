namespace Ayla;

public static class NativeCallBoundary
{
    public static void ThrowIfFailed(NativeCallStatus status)
    {
        NativeException.ThrowIfFailed(status);
    }
}
