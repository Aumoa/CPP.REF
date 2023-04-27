// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Exceptions;

public class TerminateException : Exception
{
    public TerminateException(KnownErrorCode ErrorCode, string ErrorString, Exception? InnerException = null)
        : base($"AE{(int)ErrorCode:D4}: {ErrorString}", InnerException)
    {
        this.ErrorCode = ErrorCode;
    }

    public KnownErrorCode ErrorCode { get; }
}
