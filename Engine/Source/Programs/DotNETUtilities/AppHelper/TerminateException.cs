namespace AylaEngine;

public class TerminateException : Exception
{
    public const int EC_Successfully = 0;
    public const int EC_User = 1;
    public const int EC_Arguments = 2;
    public const int EC_Internal = 3;
    public const int EC_Thirdparty = 4;
    public const int EC_Abort = 5;

    public readonly int ReturnCode;

    public TerminateException(int returnCode)
    {
        ReturnCode = returnCode;
    }

    public static TerminateException Successfully() => new(EC_Successfully);
    public static TerminateException User() => new(EC_User);
    public static TerminateException Arguments() => new(EC_Arguments);
    public static TerminateException Internal() => new(EC_Internal);
    public static TerminateException Thirdparty() => new(EC_Thirdparty);
    public static TerminateException Abort() => new(EC_Abort);
}
