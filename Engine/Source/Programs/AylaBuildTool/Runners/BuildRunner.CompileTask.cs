using static AylaEngine.Compiler;

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

        public async Task<Terminal.Output> CompileAsync(SemaphoreSlim access, Installation installation, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            await access.WaitAsync(cancellationToken);
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
            finally
            {
                access.Release();
            }
        }

        public Task Task => m_CompletionSource.Task;

        public GroupDescriptor Group => Item.Descriptor;
    }
}
