namespace AylaEngine;

public class TerminateException : Exception
{
    public const int EC_Successfully = 0;
    public const int EC_User = 1;
    public const int EC_Arguments = 2;
    public const int EC_Internal = 3;
    public const int EC_Thirdparty = 4;
    public const int EC_Abort = 5;
    public const int EC_NotSupport = 6;

    public readonly int ReturnCode;

    public string? DisplayMessage { get; }

    public TerminateException(int returnCode, string? message = null, Exception? innerException = null)
        : base(message, innerException)
    {
        ReturnCode = returnCode;
        DisplayMessage = message;
    }

    public static TerminateException Successfully(string? message = null, Exception? innerException = null) => new(EC_Successfully, message, innerException);
    public static TerminateException User(string? message = null, Exception? innerException = null) => new(EC_User, message, innerException);
    public static TerminateException Arguments(string? message = null, Exception? innerException = null) => new(EC_Arguments, message, innerException);
    public static TerminateException Internal(string? message = null, Exception? innerException = null) => new(EC_Internal, message, innerException);
    public static TerminateException Thirdparty(string? message = null, Exception? innerException = null) => new(EC_Thirdparty, message, innerException);
    public static TerminateException Abort(string? message = null, Exception? innerException = null) => new(EC_Abort, message, innerException);
    public static TerminateException NotSupport(string? message = null, Exception? innerException = null) => new(EC_NotSupport, message, innerException);
}
