namespace AylaEngine;

internal static partial class BuildRunner
{
    private sealed class BuildAction
    {
        private readonly Func<CancellationToken, Task<Terminal.Output>> m_ExecuteAsync;
        private readonly Func<Terminal.Output, string> m_FormatOutput;

        public BuildAction(Func<CancellationToken, Task<Terminal.Output>> executeAsync, Func<Terminal.Output, string> formatOutput)
        {
            m_ExecuteAsync = executeAsync;
            m_FormatOutput = formatOutput;
        }

        public async Task<BuildActionResult> ExecuteAsync(CancellationToken cancellationToken)
        {
            var output = await m_ExecuteAsync(cancellationToken);
            return new BuildActionResult(output, m_FormatOutput(output));
        }
    }

    private sealed class BuildActionResult
    {
        public BuildActionResult(Terminal.Output output, string message)
        {
            Output = output;
            Message = message;
        }

        public Terminal.Output Output { get; }

        public string Message { get; }
    }

    private sealed class BuildActionExecutor
    {
        private readonly BuildAction[] m_Actions;
        private readonly HashSet<string> m_LoggedTerminalFailures = [];
        private readonly int m_LogWidth;
        private int m_CompletedActionCount;

        public BuildActionExecutor(IEnumerable<BuildAction> actions)
        {
            m_Actions = actions.ToArray();
            m_LogWidth = m_Actions.Length switch
            {
                >= 0 and < 10 => 1,
                >= 10 and < 100 => 2,
                >= 100 and < 1000 => 3,
                >= 1000 and < 10000 => 4,
                >= 10000 and < 100000 => 5,
                _ => 6
            };
        }

        public async Task ExecuteAsync(CancellationToken cancellationToken)
        {
            var buildTasks = m_Actions.Select(action => ExecuteActionAsync(action, cancellationToken)).ToArray();
            try
            {
                await Task.WhenAll(buildTasks);
            }
            catch
            {
                foreach (var task in buildTasks.Where(p => p.IsFaulted))
                {
                    LogTaskFailure(task.Exception);
                }

                throw TerminateException.User();
            }
        }

        private async Task ExecuteActionAsync(BuildAction action, CancellationToken cancellationToken)
        {
            var result = await action.ExecuteAsync(cancellationToken);
            if (string.IsNullOrWhiteSpace(result.Message))
            {
                return;
            }

            Console.WriteLine("{0} {1}", MakeOutputPrefix(result.Output.ElapsedSeconds), result.Message);
        }

        private string MakeOutputPrefix(double elapsedSeconds)
        {
            return string.Format($"[{{0,{m_LogWidth}}}/{{1,{m_LogWidth}}} {{2,5:F1}}s]", Interlocked.Increment(ref m_CompletedActionCount), m_Actions.Length, elapsedSeconds);
        }

        private void LogTaskFailure(Exception? exception)
        {
            if (exception == null)
            {
                return;
            }

            IEnumerable<Exception> exceptions = exception is AggregateException ae
                ? ae.Flatten().InnerExceptions
                : [exception];

            foreach (var ex in exceptions)
            {
                if (ex is TerminalExecutionException terminalException)
                {
                    var output = terminalException.Output;
                    var key = $"{output.Executable}\n{output.Command}\n{output.ExitCode}";
                    lock (m_LoggedTerminalFailures)
                    {
                        if (!m_LoggedTerminalFailures.Add(key))
                        {
                            continue;
                        }
                    }

                    var logText = string.Join('\n', output.Logs.Select(l => l.Value));
                    Console.Error.WriteLine("{0} Terminal execution failed with code {1}\n{2}{3}",
                        MakeOutputPrefix(output.ElapsedSeconds),
                        output.ExitCode,
                        output.Command,
                        string.IsNullOrWhiteSpace(logText) ? string.Empty : "\n" + logText);
                }
                else if (ex is not OperationCanceledException)
                {
                    Console.Error.WriteLine(ex.Message);
                }
            }
        }
    }
}
