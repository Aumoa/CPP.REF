// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Diagnostics;

public static class Log
{
    private static readonly Thread LogThread;
    private static readonly CancellationTokenSource StopToken = new();
    private static List<LogEntry> LogEntries = new();

    static Log()
    {
        LogThread = new Thread(Run);
        LogThread.Start();
    }

    public static void Verbose(LogVerbosity Verboisty, string Format, params object?[] Args)
    {
        lock (LogThread)
        {
            LogEntries.Add(new LogEntry
            {
                Verbosity = Verboisty,
                Message = string.Format(Format, Args)
            });

            Monitor.Pulse(LogThread);
        }
    }

    public static void Action(Action LogAction)
    {
        lock (LogThread)
        {
            LogEntries.Add(new LogEntry
            {
                LogAction = LogAction
            });

            Monitor.Pulse(LogThread);
        }
    }

    public static void Trace(string Format, params object?[] Args) => Verbose(LogVerbosity.Trace, Format, Args);

    public static void Info(string Format, params object?[] Args) => Verbose(LogVerbosity.Info, Format, Args);

    public static void Warning(string Format, params object?[] Args) => Verbose(LogVerbosity.Warning, Format, Args);

    public static void Error(string Format, params object?[] Args) => Verbose(LogVerbosity.Error, Format, Args);

    public static void Critical(string Format, params object?[] Args) => Verbose(LogVerbosity.Critical, Format, Args);

    public static Task FlushAsync(CancellationToken CToken = default)
    {
        if (StopToken.IsCancellationRequested)
        {
            return Task.CompletedTask;
        }

        TaskCompletionSource Source = new();
        Action(() => Source.SetResult());
        return Source.Task.WaitAsync(CToken);
    }

    public static void Shutdown()
    {
        lock (LogThread)
        {
            StopToken.Cancel();
            Monitor.Pulse(LogThread);
        }

        LogThread.Join();
    }

    private static string VerbosityToString(LogVerbosity verbosity) => verbosity switch
    {
        LogVerbosity.Trace => "dbug",
        LogVerbosity.Info => "info",
        LogVerbosity.Warning => "warn",
        LogVerbosity.Error => "erro",
        LogVerbosity.Critical => "ftal",
        _ => "????"
    };

    private static ConsoleColor VerbosityToColor(LogVerbosity verbosity) => verbosity switch
    {
        LogVerbosity.Trace => ConsoleColor.White,
        LogVerbosity.Info => ConsoleColor.DarkGreen,
        LogVerbosity.Warning => ConsoleColor.DarkYellow,
        LogVerbosity.Error => ConsoleColor.DarkRed,
        LogVerbosity.Critical => ConsoleColor.Red,
        _ => ConsoleColor.Red
    };

    private static void Run()
    {
        List<LogEntry> Entries = new();
        Thread.CurrentThread.Name = "[Log Thread]";
        ConsoleColor ForegroundColor = Console.ForegroundColor;

        while (true)
        {
            lock (LogThread)
            {
                while (!LogEntries.Any())
                {
                    Monitor.Wait(LogThread, 100);
                    if (StopToken.Token.IsCancellationRequested)
                    {
                        break;
                    }
                }

                if (StopToken.Token.IsCancellationRequested && LogEntries.Count == 0)
                {
                    break;
                }

                // Swap buffer.
                (Entries, LogEntries) = (LogEntries, Entries);
            }

            foreach (var Entry in Entries)
            {
                if (Entry.Message is null)
                {
                    Entry.LogAction!();
                }
                else
                {
                    Console.ForegroundColor = VerbosityToColor(Entry.Verbosity);
                    Console.Write(VerbosityToString(Entry.Verbosity));
                    Console.ForegroundColor = ForegroundColor;
                    Console.WriteLine(": {0}", Entry.Message);
                }
            }

            Entries.Clear();
        }
    }
}
