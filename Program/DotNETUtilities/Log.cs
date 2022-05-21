// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Reflection;

namespace DotNETUtilities;

/// <summary>
/// 콘솔 화면에 출력하는 로그에 대한 공통 함수를 제공합니다.
/// </summary>
[LogCategory("General")]
public static class Log
{
    static Log()
    {
        new Thread(Run).Start();
    }

    /// <summary>
    /// 콘솔 화면에 로그를 출력합니다.
    /// </summary>
    /// <param name="category"> 로그 카테고리를 전달합니다. </param>
    /// <param name="verbosity"> 로그 레벨을 전달합니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Verbose(LogCategoryAttribute? category, LogVerbosity verbosity, string format, params object[] args)
    {
        lock (_entries)
        {
            _entries.Add(new LogEntry
            {
                Category = category,
                Verbosity = verbosity,
                Message = string.Format(format, args)
            });

            Monitor.Pulse(_entries);
        }
    }

    /// <summary>
    /// 로그성 행동을 로그 스레드에서 실행합니다.
    /// </summary>
    /// <param name="logAction"> 로그성 행동을 전달합니다. </param>
    public static void Action(Action logAction)
    {
        lock (_entries)
        {
            _entries.Add(new LogEntry
            {
                LogAction = logAction
            });

            Monitor.Pulse(_entries);
        }
    }

    /// <summary>
    /// <see cref="LogVerbosity.Info"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="category"> 로그 카테고리를 전달합니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Trace(LogCategoryAttribute? category, string format, params object[] args)
        => Verbose(category, LogVerbosity.Trace, format, args);

    /// <summary>
    /// <see cref="LogVerbosity.Trace"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="self"> <see cref="LogCategoryAttribute"/> 특성을 소유한 개체를 전달합니다. 없을 경우 <see langword="null"/>이 사용됩니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Trace(this object? self, string format, params object[] args)
        => Trace(self?.GetType().GetCustomAttribute<LogCategoryAttribute>(), format, args);

    /// <summary>
    /// <see cref="LogVerbosity.Info"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="category"> 로그 카테고리를 전달합니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Info(LogCategoryAttribute? category, string format, params object[] args)
        => Verbose(category, LogVerbosity.Info, format, args);

    /// <summary>
    /// <see cref="LogVerbosity.Info"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="self"> <see cref="LogCategoryAttribute"/> 특성을 소유한 개체를 전달합니다. 없을 경우 <see langword="null"/>이 사용됩니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Info(this object? self, string format, params object[] args)
        => Info(self?.GetType().GetCustomAttribute<LogCategoryAttribute>(), format, args);

    /// <summary>
    /// <see cref="LogVerbosity.Warning"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="category"> 로그 카테고리를 전달합니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Warning(LogCategoryAttribute? category, string format, params object[] args)
        => Verbose(category, LogVerbosity.Warning, format, args);

    /// <summary>
    /// <see cref="LogVerbosity.Warning"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="self"> <see cref="LogCategoryAttribute"/> 특성을 소유한 개체를 전달합니다. 없을 경우 <see langword="null"/>이 사용됩니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Warning(this object? self, string format, params object[] args)
        => Warning(self?.GetType().GetCustomAttribute<LogCategoryAttribute>(), format, args);

    /// <summary>
    /// <see cref="LogVerbosity.Error"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="category"> 로그 카테고리를 전달합니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Error(LogCategoryAttribute? category, string format, params object[] args)
        => Verbose(category, LogVerbosity.Error, format, args);

    /// <summary>
    /// <see cref="LogVerbosity.Error"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="self"> <see cref="LogCategoryAttribute"/> 특성을 소유한 개체를 전달합니다. 없을 경우 <see langword="null"/>이 사용됩니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Error(this object? self, string format, params object[] args)
        => Error(self?.GetType().GetCustomAttribute<LogCategoryAttribute>(), format, args);

    /// <summary>
    /// <see cref="LogVerbosity.Critical"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="category"> 로그 카테고리를 전달합니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Critical(LogCategoryAttribute? category, string format, params object[] args)
        => Verbose(category, LogVerbosity.Critical, format, args);

    /// <summary>
    /// <see cref="LogVerbosity.Critical"/> 로그 레벨로 로그를 출력합니다.
    /// </summary>
    /// <param name="self"> <see cref="LogCategoryAttribute"/> 특성을 소유한 개체를 전달합니다. 없을 경우 <see langword="null"/>이 사용됩니다. </param>
    /// <param name="format"> 서식 텍스트를 전달합니다. </param>
    /// <param name="args"> 서식 매개변수를 전달합니다. </param>
    public static void Critical(this object? self, string format, params object[] args)
        => Critical(self?.GetType().GetCustomAttribute<LogCategoryAttribute>(), format, args);

    /// <summary>
    /// 모든 대기 로그를 출력하고 로그 스레드를 종료합니다.
    /// </summary>
    public static void Shutdown()
    {
        lock (_entries)
        {
            _running = false;
            Monitor.Pulse(_entries);
        }
    }

    private static bool _running = true;
    private static List<LogEntry> _entries = new();

    private static void Swap<T>(ref T lhs, ref T rhs)
    {
        (lhs, rhs) = (rhs, lhs);
    }

    private static void Run()
    {
        List<LogEntry> entries = new();
        Thread.CurrentThread.Name = "[Log Thread]";

        while (_running)
        {
            lock (_entries)
            {
                while (!_entries.Any())
                {
                    Monitor.Wait(_entries, 100);
                    if (!_running)
                    {
                        break;
                    }
                }

                Swap(ref entries, ref _entries);
            }

            foreach (var entry in entries)
            {
                if (entry.Verbosity is null)
                {
                    entry.LogAction!();
                }
                else
                {
                    Console.WriteLine(entry.Message);
                }
            }

            entries.Clear();
        }
    }
}
