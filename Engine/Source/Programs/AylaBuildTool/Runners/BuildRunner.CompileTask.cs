namespace AylaEngine;

internal static partial class BuildRunner
{
    internal class CompileTask : ITask
    {
        public readonly CppCompileCommand Command;
        private readonly Task[] m_PrerequisiteTasks;
        private readonly TaskCompletionSource m_CompletionSource = new();

        public CompileTask(CppCompileCommand command, params Task[] prerequisiteTasks)
        {
            Command = command;
            m_PrerequisiteTasks = prerequisiteTasks;
        }

        public async Task<Terminal.Output> CompileAsync(Installation installation, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            try
            {
                await Task.WhenAll(m_PrerequisiteTasks);
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
