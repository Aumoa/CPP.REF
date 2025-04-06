using static AylaEngine.Compiler;

namespace AylaEngine;

internal static partial class BuildRunner
{
    private class ModuleTask : ITask
    {
        public readonly ModuleRulesResolver Resolver;
        public readonly CompileTask[] NeedCompileTasks;
        public readonly GenerateReflectionHeaderTask[] GRHTasks;

        private readonly CompileItem[] m_AllCompiles;
        private readonly TaskCompletionSource m_CompletionSource = new();

        public ModuleTask(ModuleRulesResolver resolver, CompileItem[] allCompiles, CompileTask[] needCompiles, GenerateReflectionHeaderTask[] grhTasks)
        {
            Resolver = resolver;
            NeedCompileTasks = needCompiles;
            m_AllCompiles = allCompiles;
            GRHTasks = grhTasks;
        }

        public async Task<Terminal.Output> LinkAsync(SemaphoreSlim access, IList<ModuleTask> moduleTasks, Installation installation, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            await Task.WhenAll(NeedCompileTasks.Select(p => p.Task));

            foreach (var name in Resolver.DependencyModuleNames)
            {
                await moduleTasks.Where(p => p.Resolver.Name == name).First().Task;
            }

            await access.WaitAsync(cancellationToken);
            try
            {
                var linker = await installation.SpawnLinkerAsync(targetInfo, cancellationToken);
                var output = await linker.LinkAsync(Resolver, m_AllCompiles, cancellationToken);
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

        public GroupDescriptor Group => Resolver.Group;
    }
}
