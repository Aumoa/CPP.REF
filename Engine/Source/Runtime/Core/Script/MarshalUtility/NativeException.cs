using System.Runtime.InteropServices;

namespace Ayla;

public sealed class NativeException : Exception
{
    public string NativeTypeName { get; }
    public string NativeDetails { get; }
    public string NativeSourceFile { get; }
    public string NativeSourceFunction { get; }
    public int NativeSourceLine { get; }

    private NativeException(
        string nativeTypeName,
        string message,
        string nativeDetails,
        string nativeSourceFile,
        string nativeSourceFunction,
        int nativeSourceLine)
        : base(message)
    {
        NativeTypeName = nativeTypeName;
        NativeDetails = nativeDetails;
        NativeSourceFile = nativeSourceFile;
        NativeSourceFunction = nativeSourceFunction;
        NativeSourceLine = nativeSourceLine;
    }

    public static void ThrowIfFailed(NativeCallStatus status)
    {
        if (status == NativeCallStatus.Success)
        {
            return;
        }

        NativeExceptionInfo info = GetLastException__Injected();
        ClearLastException__Injected();
        throw FromInfo(info);
    }

    private static NativeException FromInfo(NativeExceptionInfo info)
    {
        string nativeTypeName = info.m_TypeName.AsManaged();
        string message = info.m_Message.AsManaged();
        string nativeDetails = info.m_Details.AsManaged();
        string nativeSourceFile = info.m_SourceFile.AsManaged();
        string nativeSourceFunction = info.m_SourceFunction.AsManaged();

        return new NativeException(
            nativeTypeName,
            message,
            nativeDetails,
            nativeSourceFile,
            nativeSourceFunction,
            info.m_SourceLine);
    }

    public override string ToString()
    {
        return NativeDetails.Length == 0 ? base.ToString() : NativeDetails;
    }

    [DllImport("Core", EntryPoint = "Ayla__NativeExceptionInterop__GetLastException__Injected")]
    private static extern NativeExceptionInfo GetLastException__Injected();

    [DllImport("Core", EntryPoint = "Ayla__NativeExceptionInterop__ClearLastException__Injected")]
    private static extern void ClearLastException__Injected();
}
