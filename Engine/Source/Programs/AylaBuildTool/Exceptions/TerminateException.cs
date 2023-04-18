// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Exceptions;

public class TerminateException : Exception
{
    public enum KnownErrorCode
    {
        NotSupportedType = 1,
        ModuleDuplicate = 2,
        MultipleClassDefinitions = 3,
        ConstructorNotFound = 4,
        TargetDuplicate = 5,
        TargetNotFound = 6,
        DependencyModuleNotFound = 7,
        ModuleNotFound = 8,
        CompileError = 9,
        InvalidModule = 10
    }

    public TerminateException(int ErrorCode, string ErrorStringFormat, params object?[] Args)
        : base($"AE{ErrorCode:D4}: {string.Format(ErrorStringFormat, Args)}")
    {
        this.ErrorCode = ErrorCode;
    }

    public int ErrorCode { get; }
}
