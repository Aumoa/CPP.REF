using System.Reflection;

namespace AylaEngine;

internal static class BuildRulesExceptionHandler
{
    public static T Evaluate<T>(string message, Func<T> action)
    {
        try
        {
            return action();
        }
        catch (Exception e) when (ShouldConvert(e))
        {
            throw ToUserException(message, e);
        }
    }

    private static bool ShouldConvert(Exception exception)
    {
        return exception is not OperationCanceledException and not TerminateException;
    }

    private static TerminateException ToUserException(string message, Exception exception)
    {
        return TerminateException.User(message, Unwrap(exception));
    }

    private static Exception Unwrap(Exception exception)
    {
        while (exception is TargetInvocationException or TypeInitializationException)
        {
            if (exception.InnerException == null)
            {
                return exception;
            }

            exception = exception.InnerException;
        }

        return exception;
    }
}
