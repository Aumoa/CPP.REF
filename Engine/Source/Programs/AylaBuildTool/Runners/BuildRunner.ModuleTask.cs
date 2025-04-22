using static AylaEngine.CppCompiler;

namespace AylaEngine;

internal static partial class BuildRunner
{
    private class ModuleTask : ITask
    {
        public readonly ModuleRulesResolver Resolver;
        public readonly CompileTask[] NeedCompileTasks;

        private readonly CompileItem[] m_AllCompiles;
        private readonly TaskCompletionSource m_CompletionSource = new();

        public ModuleTask(ModuleRulesResolver resolver, CompileItem[] allCompiles, CompileTask[] needCompiles)
        {
            Resolver = resolver;
            NeedCompileTasks = needCompiles;
            m_AllCompiles = allCompiles;
        }

        public bool NeedLink(ITargetInfo targetInfo)
        {
            if (NeedCompileTasks.Length > 0)
            {
                return true;
            }

            var outputFileName = Resolver.Group.OutputFileName(targetInfo, Resolver.Name, Resolver.Rules.Type, FolderPolicy.PathType.Current);
            if (File.Exists(outputFileName) == false)
            {
                return true;
            }

            return false;
        }

        public async Task<Terminal.Output> LinkAsync(IList<ModuleTask> moduleTasks, Installation installation, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            await Task.WhenAll(NeedCompileTasks.Select(p => p.Task));

            foreach (var name in Resolver.DependencyModuleNames)
            {
                await moduleTasks.Where(p => p.Resolver.Name == name).First().Task;
            }

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
        }

        public void SetComplete()
        {
            m_CompletionSource.SetResult();
        }

        public Task Task => m_CompletionSource.Task;

        public GroupDescriptor Group => Resolver.Group;
    }
}
