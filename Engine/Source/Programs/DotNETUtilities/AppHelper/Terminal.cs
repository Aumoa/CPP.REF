using Spectre.Console;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace AylaEngine;

public static class Terminal
{
    [Flags]
    public enum Logging
    {
        None,
        StdOut = 0x01,
        StdErr = 0x02,
        All = StdOut | StdErr
    }

    public record Options
    {
        public required string Executable { get; init; }

        public string WorkingDirectory { get; init; } = string.Empty;

        public Logging Logging { get; init; } = Logging.StdOut;
    }

    public record Log
    {
        public required string Value { get; init; }

        public required Verbose Verbosity { get; init; } = Verbose.Info;
    }

    public record Output
    {
        public required string Executable { get; init; }

        public required string Command { get; init; }

        public required int ExitCode { get; init; } = -1;

        public required Log[] Logs { get; init; }

        public required Log[] StdOut { get; init; }

        public required Log[] StdErr { get; init; }

        public bool IsCompletedSuccessfully => ExitCode == 0;

        public bool IsFailure => ExitCode != 0;
    }

    public enum Verbose
    {
        Info,
        Warning,
        Error,
        Critical,
    }

    private static readonly HashSet<char> MarkupSeparator =
    [
        '[', ']', '{', '}', '(', ')',
        '-', '.', ',',
        '/', '\\', '|',
        '~', '!', '@', '#', '$', '%', '^', '&', '*',
        '+', '=',
        '`', '"', '\'',
        '\t', ' ', '\0'
    ];

    public static async ValueTask<Output> ExecuteCommandAsync(string command, Options options, CancellationToken cancellationToken = default)
    {
        var process = StartProcess(options.Executable, command, options.WorkingDirectory);

        List<Log> logs = [];
        List<Log> stdout = [];
        List<Log> stderr = [];
        process.OutputDataReceived += (_, e) =>
        {
            if (string.IsNullOrEmpty(e.Data) == false)
            {
                lock (logs)
                {
                    var l = new Log
                    {
                        Value = e.Data,
                        Verbosity = Verbose.Info
                    };

                    logs.Add(l);
                    stdout.Add(l);

                    if ((options.Logging | Logging.StdOut) != 0)
                    {
                        AnsiConsole.MarkupLine(GetMarkupText(e.Data.EscapeMarkup()));
                    }
                }
            }
        };

        process.ErrorDataReceived += (_, e) =>
        {
            if (string.IsNullOrEmpty(e.Data) == false)
            {
                lock (logs)
                {
                    var l = new Log
                    {
                        Value = e.Data,
                        Verbosity = Verbose.Error
                    };

                    logs.Add(l);
                    stderr.Add(l);

                    if ((options.Logging & Logging.StdErr) != 0)
                    {
                        AnsiConsole.MarkupLine("[red]{0}[/]", e.Data.EscapeMarkup());
                    }
                }
            }
        };

        process.BeginOutputReadLine();
        process.BeginErrorReadLine();
        await using (cancellationToken.Register(() => process.Kill()))
        {
            await process.WaitForExitAsync(cancellationToken);
        }

        return new Output
        {
            Executable = options.Executable,
            Command = command,
            ExitCode = process.ExitCode,
            Logs = logs.ToArray(),
            StdOut = stdout.ToArray(),
            StdErr = stderr.ToArray()
        };

        string GetMarkupText(string value)
        {
            if (WholeContent("warning") || WholeContent("warn"))
            {
                return $"[yellow]{value.EscapeMarkup()}[/]";
            }
            else if (WholeContent("success") || WholeContent("successfully"))
            {
                return $"[green]{value.EscapeMarkup()}[/]";
            }
            else if (WholeContent("error") || WholeContent("err"))
            {
                return $"[red]{value.EscapeMarkup()}[/]";
            }
            else if (WholeContent("crit") || WholeContent("critical") || WholeContent("fatal"))
            {
                return $"[red][b]{value.EscapeMarkup()}[/][/]";
            }

                return value;

            bool WholeContent(string content)
            {
                int indexOf = value.IndexOf(content, StringComparison.OrdinalIgnoreCase);
                if (indexOf == -1)
                {
                    return false;
                }

                if ((indexOf != 0 && MarkupSeparator.Contains(value[indexOf - 1])) ||
                    (indexOf != value.Length - 1) && MarkupSeparator.Contains(value[indexOf + 1]))
                {
                    return true;
                }

                return false;
            }
        }
    }

    private static Process StartProcess(string? executable, string command, string workingDirectory = ".")
    {
        var processInfo = new ProcessStartInfo
        {
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            RedirectStandardInput = false,
            UseShellExecute = false,
            WorkingDirectory = workingDirectory
        };

        if (executable == null)
        {
            processInfo.FileName = RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? "cmd.exe" : "/bin/bash";
            processInfo.Arguments = RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? $"/c {command}" : $"-c \"{command}\"";
        }
        else
        {
            processInfo.FileName = executable;
            processInfo.Arguments = command;
        }

        var process = Process.Start(processInfo);
        if (process == null)
        {
            AnsiConsole.MarkupLine("[red]Internal error: Cannot start [b]{0}[.] process.[/]", processInfo.FileName);
            throw TerminateException.Internal();
        }

        return process;
    }

    public static async ValueTask<int> ExitCode(this ValueTask<Output> result)
    {
        var output = await result;
        return output.ExitCode;
    }
}
