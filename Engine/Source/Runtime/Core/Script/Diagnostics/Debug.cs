// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Runtime.InteropServices;

namespace Ayla;

public static class Debug
{
    [DllImport("Core", CharSet = CharSet.Unicode)]
    private static extern void Injected__Ayla__Debug__Injected_Log(string category, int logLevel, string message);

    public static void Log(string category, LogVerbosity logLevel, string message)
    {
        Injected__Ayla__Debug__Injected_Log(category, (int)logLevel, message);
    }

	public static void LogFormat(string category, LogVerbosity logLevel, string format, params ReadOnlySpan<object?> args)
	{
		Log(category, logLevel, string.Format(format, args));
	}

	public static void LogVerbose(string category, string message)
	{
		Log(category, LogVerbosity.Verbose, message);
	}

	public static void LogVerboseFormat(string category, string format, params ReadOnlySpan<object?> args)
	{
		LogFormat(category, LogVerbosity.Verbose, string.Format(format, args));
	}

	public static void LogInfo(string category, string message)
	{
		Log(category, LogVerbosity.Info, message);
	}

	public static void LogInfoFormat(string category, string format, params ReadOnlySpan<object?> args)
	{
		LogFormat(category, LogVerbosity.Info, string.Format(format, args));
	}

	public static void LogWarning(string category, string message)
	{
		Log(category, LogVerbosity.Warning, message);
	}

	public static void LogWarningFormat(string category, string format, params ReadOnlySpan<object?> args)
	{
		LogFormat(category, LogVerbosity.Warning, string.Format(format, args));
	}

	public static void LogError(string category, string message)
	{
		Log(category, LogVerbosity.Error, message);
	}

	public static void LogErrorFormat(string category, string format, params ReadOnlySpan<object?> args)
	{
		LogFormat(category, LogVerbosity.Error, string.Format(format, args));
	}

	public static void LogCritical(string category, string message)
	{
		Log(category, LogVerbosity.Critical, message);
	}

	public static void LogCriticalFormat(string category, string format, params ReadOnlySpan<object?> args)
	{
		LogFormat(category, LogVerbosity.Critical, string.Format(format, args));
	}
}
