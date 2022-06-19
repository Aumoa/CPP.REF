// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

internal class CompilationException : Exception
{
    public enum ErrorCode
    {
        None = 0,
        InvalidSCLASS = 1,
        InvalidClassKey = 2,
        SyntaxError = 3,
        RuleError = 4,
    }

    public CompilationException(ErrorCode errorCode, string message, int line)
        : base(FormatMessage(errorCode, message, line))
    {
    }

    private static string FormatMessage(ErrorCode errorCode, string message, int line)
    {
        return string.Format("{0}: \"{1}\" in line {2}", errorCode, message, line);
    }
}
