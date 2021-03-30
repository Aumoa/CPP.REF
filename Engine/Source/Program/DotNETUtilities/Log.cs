// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

/// <summary>
/// 파일 및 콘솔 출력 창에 로그를 기록합니다.
/// </summary>
public static class Log
{
    //static FileReference LogFile;

    /// <summary>
    /// 일반적인 로그 형태를 기록합니다.
    /// </summary>
    /// <param name="InLogText"> 텍스트를 전달합니다. </param>
    public static void TraceLog(string InLogText)
    {
        string LogText = $"Log: {InLogText}";

        Console.WriteLine(LogText);
    }

    /// <summary>
    /// 경고 형태의 로그를 기록합니다.
    /// </summary>
    /// <param name="InWarningText"> 텍스트를 전달합니다. </param>
    public static void TraceWarning(string InWarningText)
    {
        string WarningText = $"Warning: {InWarningText}";

        Console.WriteLine(WarningText);
    }
}