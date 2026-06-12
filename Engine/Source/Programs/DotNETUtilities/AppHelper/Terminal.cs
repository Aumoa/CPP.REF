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

        public IEnumerable<KeyValuePair<string, string>> Environments { get; init; } = [];
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

        public double ElapsedSeconds { get; init; } = 0.0;

        public bool IsCompletedSuccessfully => ExitCode == 0;

        public bool IsFailure => ExitCode != 0;

        public static Output Success(string executable, string message)
        {
            var logs = new Log[]
            {
                new Log
                {
                    Value = message,
                    Verbosity = Verbose.Info
                }
            };

            return new Output
            {
                Executable = executable,
                Command = string.Empty,
                ExitCode = 0,
                Logs = logs,
                StdOut = logs,
                StdErr = []
            };
        }

        public Output AppendFirst(params Log[] logs)
        {
            return this with
            {
                Logs = [.. logs, .. Logs],
                StdOut = [.. logs.Where(l => l.Verbosity is Verbose.Info or Verbose.Warning), .. StdOut],
                StdErr = [.. logs.Where(l => l.Verbosity is Verbose.Error or Verbose.Critical), .. StdErr]
            };
        }

        public Output AppendLast(params Log[] logs)
        {
            return this with
            {
                Logs = [.. Logs, .. logs],
                StdOut = [.. StdOut, .. logs.Where(l => l.Verbosity is Verbose.Info or Verbose.Warning)],
                StdErr = [.. StdErr, .. logs.Where(l => l.Verbosity is Verbose.Error or Verbose.Critical)]
            };
        }

        public Output Replace(params Log[] logs)
        {
            return this with
            {
                Logs = [.. logs],
                StdOut = [.. logs.Where(l => l.Verbosity is Verbose.Info or Verbose.Warning)],
                StdErr = [.. logs.Where(l => l.Verbosity is Verbose.Error or Verbose.Critical)]
            };
        }
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
        return await ExecuteProcessAsync(
            command,
            options,
            () => StartProcess(options.Executable, command, options.WorkingDirectory, options.Environments),
            cancellationToken);
    }

    public static async ValueTask<Output> ExecuteCommandAsync(IEnumerable<string> arguments, Options options, CancellationToken cancellationToken = default)
    {
        var argumentArray = arguments.ToArray();
        var command = string.Join(' ', argumentArray.Select(FormatArgumentForDisplay));
        return await ExecuteProcessAsync(
            command,
            options,
            () => StartProcess(options.Executable, argumentArray, options.WorkingDirectory, options.Environments),
            cancellationToken);
    }

    private static async ValueTask<Output> ExecuteProcessAsync(string command, Options options, Func<Process> startProcess, CancellationToken cancellationToken)
    {
        var sw = Stopwatch.StartNew();
        var process = startProcess();

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

                    if ((options.Logging & Logging.StdOut) != 0)
                    {
                        Console.WriteLine(e.Data);
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
                        Console.Error.WriteLine(e.Data);
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
        sw.Stop();

        return new Output
        {
            Executable = options.Executable,
            Command = command,
            ExitCode = process.ExitCode,
            Logs = logs.ToArray(),
            StdOut = stdout.ToArray(),
            StdErr = stderr.ToArray(),
            ElapsedSeconds = sw.Elapsed.TotalSeconds
        };
    }

    private static string FormatArgumentForDisplay(string value)
    {
        if (value.Length == 0)
        {
            return "\"\"";
        }

        if (value.Any(char.IsWhiteSpace) == false && value.Contains('"') == false)
        {
            return value;
        }

        return "\"" + value.Replace("\"", "\\\"") + "\"";
    }

    private static Process StartProcess(string? executable, string command, string workingDirectory, IEnumerable<KeyValuePair<string, string>> environments)
    {
        var processInfo = CreateProcessStartInfo(workingDirectory, environments);

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

        return StartProcess(processInfo);
    }

    private static Process StartProcess(string? executable, IEnumerable<string> arguments, string workingDirectory, IEnumerable<KeyValuePair<string, string>> environments)
    {
        if (executable == null)
        {
            Console.WriteLine("Internal error: Cannot start argument-list process without an executable.");
            throw TerminateException.Internal();
        }

        var processInfo = CreateProcessStartInfo(workingDirectory, environments);
        processInfo.FileName = executable;

        foreach (var argument in arguments)
        {
            processInfo.ArgumentList.Add(argument);
        }

        return StartProcess(processInfo);
    }

    private static ProcessStartInfo CreateProcessStartInfo(string workingDirectory, IEnumerable<KeyValuePair<string, string>> environments)
    {
        var processInfo = new ProcessStartInfo
        {
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            RedirectStandardInput = false,
            UseShellExecute = false,
            WorkingDirectory = workingDirectory
        };

        foreach (var env in environments)
        {
            processInfo.Environment[env.Key] = env.Value;
        }

        return processInfo;
    }

    private static Process StartProcess(ProcessStartInfo processInfo)
    {
        var process = Process.Start(processInfo);
        if (process == null)
        {
            Console.WriteLine("Internal error: Cannot start {0} process.", processInfo.FileName);
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
