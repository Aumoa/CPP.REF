using System.Runtime.InteropServices;

namespace Ayla;

public static class ManagedExceptionInterop
{
    public static unsafe NativeCallStatus Capture(global::System.Exception exception)
    {
        string typeName = exception.GetType().FullName ?? exception.GetType().Name;
        string message = exception.Message;
        string details = exception.ToString();
        ulong nativeExceptionToken = exception is NativeException nativeException ? nativeException.NativeExceptionToken : 0;

        fixed (char* typeNamePtr = typeName)
        fixed (char* messagePtr = message)
        fixed (char* detailsPtr = details)
        {
            return CaptureException__Injected(
                new ManagedStringWrapper(typeNamePtr, typeName.Length),
                new ManagedStringWrapper(messagePtr, message.Length),
                new ManagedStringWrapper(detailsPtr, details.Length),
                nativeExceptionToken);
        }
    }

    [DllImport("Core", EntryPoint = "Ayla__ManagedExceptionInterop__CaptureException__Injected")]
    private static extern NativeCallStatus CaptureException__Injected(
        ManagedStringWrapper typeName,
        ManagedStringWrapper message,
        ManagedStringWrapper details,
        ulong nativeExceptionToken);
}
