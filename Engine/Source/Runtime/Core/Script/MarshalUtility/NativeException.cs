using System.Runtime.InteropServices;

namespace Ayla;

public sealed class NativeException : Exception
{
    public string NativeTypeName { get; }
    public string NativeDetails { get; }
    public string NativeSourceFile { get; }
    public string NativeSourceFunction { get; }
    public int NativeSourceLine { get; }
    private ulong m_NativeExceptionToken;
    internal ulong NativeExceptionToken => m_NativeExceptionToken;

    private NativeException(
        string nativeTypeName,
        string message,
        string nativeDetails,
        string nativeSourceFile,
        string nativeSourceFunction,
        int nativeSourceLine,
        ulong nativeExceptionToken)
        : base(message)
    {
        NativeTypeName = nativeTypeName;
        NativeDetails = nativeDetails;
        NativeSourceFile = nativeSourceFile;
        NativeSourceFunction = nativeSourceFunction;
        NativeSourceLine = nativeSourceLine;
        m_NativeExceptionToken = nativeExceptionToken;
    }

    ~NativeException()
    {
        if (m_NativeExceptionToken == 0)
        {
            return;
        }

        try
        {
            ReleaseCapturedException__Injected(m_NativeExceptionToken);
        }
        catch
        {
        }
    }

    internal ulong DetachNativeExceptionToken()
    {
        ulong nativeExceptionToken = m_NativeExceptionToken;
        m_NativeExceptionToken = 0;
        GC.SuppressFinalize(this);
        return nativeExceptionToken;
    }

    public static void ThrowIfFailed(NativeCallStatus status)
    {
        if (status == NativeCallStatus.Succeeded)
        {
            return;
        }

        NativeExceptionInfo info = GetLastException__Injected();
        ClearLastException__Injected();

        if (info.m_ManagedExceptionToken != 0)
        {
            ReleaseCaptured(info.m_ExceptionToken);
            ManagedExceptionInterop.ThrowCaptured(info.m_ManagedExceptionToken);
            return;
        }

        NativeException exception;
        try
        {
            exception = FromInfo(info);
        }
        catch
        {
            ReleaseCaptured(info.m_ExceptionToken);
            ManagedExceptionInterop.ReleaseCaptured(info.m_ManagedExceptionToken);
            throw;
        }

        throw exception;
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
            info.m_SourceLine,
            info.m_ExceptionToken);
    }

    internal static void ReleaseCaptured(ulong nativeExceptionToken)
    {
        if (nativeExceptionToken == 0)
        {
            return;
        }

        ReleaseCapturedException__Injected(nativeExceptionToken);
    }

    public override string ToString()
    {
        return NativeDetails.Length == 0 ? base.ToString() : NativeDetails;
    }

    [DllImport("Core", EntryPoint = "Ayla__NativeExceptionInterop__GetLastException__Injected")]
    private static extern NativeExceptionInfo GetLastException__Injected();

    [DllImport("Core", EntryPoint = "Ayla__NativeExceptionInterop__ReleaseCapturedException__Injected")]
    private static extern void ReleaseCapturedException__Injected(ulong exceptionToken);

    [DllImport("Core", EntryPoint = "Ayla__NativeExceptionInterop__ClearLastException__Injected")]
    private static extern void ClearLastException__Injected();
}
