namespace AylaEngine;

internal class TerminateException(int exitCode) : Exception
{
    public const int EC_Success = 0;
    public const int EC_User = 1;

    public readonly int ExitCode = exitCode;

    public static TerminateException Success()
    {
        return new TerminateException(EC_Success);
    }

    public static TerminateException User()
    {
        return new TerminateException(EC_User);
    }
}
