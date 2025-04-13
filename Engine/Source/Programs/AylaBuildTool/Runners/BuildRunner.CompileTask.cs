using static AylaEngine.CppCompiler;

namespace AylaEngine;

internal static partial class BuildRunner
{
    private class CompileTask : ITask
    {
        public readonly CompileItem Item;
        private readonly TaskCompletionSource m_CompletionSource = new();

        public CompileTask(CompileItem item)
        {
            Item = item;
        }

        public async Task<Terminal.Output> CompileAsync(Installation installation, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            try
            {
                var compiler = await installation.SpawnCompilerAsync(targetInfo, cancellationToken);
                var output = await compiler.CompileAsync(Item, cancellationToken);
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

        public GroupDescriptor Group => Item.Descriptor;
    }
}
