using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public readonly struct NativeExceptionInfo
{
    public readonly ManagedStringWrapper m_TypeName;
    public readonly ManagedStringWrapper m_Message;
    public readonly ManagedStringWrapper m_Details;
    public readonly ManagedStringWrapper m_SourceFile;
    public readonly ManagedStringWrapper m_SourceFunction;
    public readonly int m_SourceLine;
    public readonly ulong m_ExceptionToken;
    public readonly ulong m_ManagedExceptionToken;
}
