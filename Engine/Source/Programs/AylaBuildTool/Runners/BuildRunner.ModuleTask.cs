using System.Diagnostics;
namespace AylaEngine;

internal static partial class BuildRunner
{
    internal class ModuleTask : ITask
    {
        public readonly ModuleRulesResolver Resolver;
        public readonly CompileTask[] NeedCompileTasks;

        private readonly Installation m_Installation;
        private readonly CppCompileCommand[] m_AllCompiles;
        private readonly TaskCompletionSource m_CompletionSource = new();

        public ModuleTask(Installation installation, ModuleRulesResolver resolver, CppCompileCommand[] allCompiles, CompileTask[] needCompiles)
        {
            Resolver = resolver;
            NeedCompileTasks = needCompiles;
            m_Installation = installation;
            m_AllCompiles = allCompiles;
        }

        public bool NeedLink(ITargetInfo targetInfo)
        {
            if (NeedCompileTasks.Length > 0)
            {
                return true;
            }

            var outputFileName = Resolver.Group.OutputFileName(m_Installation, targetInfo, Resolver.Name, Resolver.Rules.Type, FolderPolicy.PathType.Current);
            if (File.Exists(outputFileName) == false)
            {
                return true;
            }

            return false;
        }

        public async Task BuildCMakeAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            var project = Resolver.Project;
            var rule = project.GetRule(targetInfo);
            if (rule.Type != ModuleType.ThirdParty)
            {
                return;
            }

            var options = new Terminal.Options
            {
                Executable = "cmake",
                Logging = Terminal.Logging.All
            };

            var cmakeSource = Path.GetFullPath(Path.Combine(project.SourceDirectory, rule.ThirdParty.CMakeSource));
            var cmakeIntDir = project.Group.Intermediate(project.Name, targetInfo.Platform, FolderPolicy.PathType.Current);
            var cmakeOutDir = project.Group.Output(targetInfo, FolderPolicy.PathType.Current);
            string[] ps =
            [
                $"-S \"{cmakeSource}\"",
                $"-B \"{cmakeIntDir}\"",
                $"-DCMAKE_BUILD_TYPE=Release",
                $"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=\"{cmakeOutDir}\"",
                $"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=\"{cmakeOutDir}\"",
                $"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG=\"{cmakeOutDir}\""
            ];
            var output = await Terminal.ExecuteCommandAsync(string.Join(' ', ps), options, cancellationToken);
            TerminalExecutionException.ThrowIfFailure(output);

            ps =
            [
                $"--build \"{cmakeIntDir}\"",
                $"--config Release",
                $"--",
                $"-p:OutDir=\"{cmakeOutDir}/\""
            ];
            output = await Terminal.ExecuteCommandAsync(string.Join(' ', ps), options, cancellationToken);
            TerminalExecutionException.ThrowIfFailure(output);
            m_CompletionSource.SetResult();
        }

        public async Task<Terminal.Output> LinkAsync(IReadOnlyList<ModuleTask> moduleTasks, Installation installation, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            try
            {
                await Task.WhenAll(NeedCompileTasks.Select(p => p.Task));

                var project = Resolver.Project;
                var rule = project.GetRule(targetInfo);
                if (rule.Type != ModuleType.ThirdParty)
                {
                    foreach (var name in Resolver.DependencyModuleNames)
                    {
                        var task = moduleTasks.Where(p => p.Resolver.Name == name).FirstOrDefault();
                        if (task != null)
                        {
                            await task.Task;
                        }
                    }

                    var linker = await installation.SpawnLinkerAsync(targetInfo, cancellationToken);
                    var output = await linker.LinkAsync(Resolver, m_AllCompiles, cancellationToken);
                    TerminalExecutionException.ThrowIfFailure(output);
                    m_CompletionSource.SetResult();
                    return output;
                }
                else
                {
                    m_CompletionSource.SetResult();
                    return Terminal.Output.Success("cmake", Resolver.Name);
                }
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
