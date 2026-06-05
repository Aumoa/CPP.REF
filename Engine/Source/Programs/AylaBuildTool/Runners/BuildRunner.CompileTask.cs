namespace AylaEngine;

internal static partial class BuildRunner
{
    private class CompileTask : ITask
    {
        public readonly CppCompileCommand Command;
        private readonly TaskCompletionSource m_CompletionSource = new();

        public CompileTask(CppCompileCommand command)
        {
            Command = command;
        }

        public async Task<Terminal.Output> CompileAsync(Installation installation, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            try
            {
                var compiler = await installation.SpawnCompilerAsync(targetInfo, cancellationToken);
                var output = await compiler.CompileAsync(Command, cancellationToken);
                TerminalExecutionException.ThrowIfFailure(output);
                m_CompletionSource.SetResult();
                return output;
            }
            catch (OperationCanceledException)
            {
                m_CompletionSource.SetCanceled();
                throw;
            }
            catch (Exception e)
            {
                m_CompletionSource.SetException(e);
                throw;
            }
        }

        public Task Task => m_CompletionSource.Task;

        public GroupDescriptor Group => Command.Descriptor;
    }
}
