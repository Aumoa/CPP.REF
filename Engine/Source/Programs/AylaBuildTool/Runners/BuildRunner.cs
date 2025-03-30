using System.Diagnostics;
using Spectre.Console;
using static AylaEngine.Compiler;
using static AylaEngine.Terminal;

namespace AylaEngine;

internal static class BuildRunner
{
    private interface ITask
    {
        GroupDescriptor Group { get; }
    }

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

    public static async ValueTask RunAsync(BuildOptions options, CancellationToken cancellationToken)
    {
        string? projectPath = null;
        if (options.ProjectFile != null)
        {
            projectPath = Path.GetDirectoryName(options.ProjectFile);
        }

        var buildTarget = new TargetInfo
        {
            Platform = PlatformInfo.Win64,
            Editor = true,
            Config = options.Config
        };

        var solution = await Solution.ScanProjectsAsync(Global.EngineDirectory, projectPath, cancellationToken);
        Dictionary<GroupDescriptor, int> compilationTaskCounts = [];
        var primaryGroup = solution.EngineProjects.First().Descriptor;
        IEnumerable<Project> targetProjects;
        if (string.IsNullOrEmpty(options.Target))
        {
            targetProjects = solution.AllProjects;
        }
        else
        {
            var targetProject = solution.FindProject(options.Target);
            if (targetProject == null)
            {
                AnsiConsole.MarkupLine("[red]Target '{0}' is not found in solution.[/]", options.Target);
                throw TerminateException.User();
            }

            if (targetProject is not ModuleProject mp)
            {
                AnsiConsole.MarkupLine("[red]Target '{0}' is not module project. Non module project must be build with specified build program.[/]", options.Target);
                throw TerminateException.User();
            }

            var resolver = new ModuleRulesResolver(buildTarget, solution, ModuleRules.New(mp.RuleType, buildTarget), mp.Descriptor, primaryGroup);
            var depends = solution.FindDepends(resolver.DependencyModuleNames);
            targetProjects = depends.Append(targetProject);
        }

        var installation = new VisualStudioInstallation();
        int compiled = 0;
        int totalActions = 0;
        int log = 1;

        List<ModuleTask> moduleTasks = [];

        foreach (var project in targetProjects)
        {
            if (project is ModuleProject mp)
            {
                var resolver = new ModuleRulesResolver(buildTarget, solution, ModuleRules.New(mp.RuleType, buildTarget), mp.Descriptor, primaryGroup);
                List<CompileItem> allCompiles = [];
                List<CompileTask> needCompiles = [];

                foreach (var sourceCode in mp.GetSourceCodes())
                {
                    if (sourceCode.Type == SourceCodeType.SourceCode)
                    {
                        var item = new Compiler.CompileItem
                        {
                            Resolver = resolver,
                            SourceCode = sourceCode,
                            Descriptor = project.Descriptor
                        };

                        var intDir = item.Descriptor.Intermediate(item.Resolver.Name, buildTarget, FolderPolicy.PathType.Current);
                        var fileName = Path.GetFileName(item.SourceCode.FilePath);
                        var cacheFileName = Path.Combine(intDir, fileName + ".cache");
                        var depsFileName = Path.Combine(intDir, fileName + ".deps.json");
                        var cached = await SourceCodeCache.MakeCachedAsync(item.SourceCode.FilePath, mp.RuleFilePath, depsFileName, resolver.DependRuleFilePaths, cancellationToken);
                        allCompiles.Add(item);
                        if (File.Exists(cacheFileName) == false ||
                            SourceCodeCache.LoadCached(cacheFileName).IsModified(cached))
                        {
                            needCompiles.Add(new CompileTask(item));
                        }
                    }
                }

                moduleTasks.Add(new ModuleTask(resolver, allCompiles.ToArray(), needCompiles.ToArray()));
            }
        }

        int hardwareConcurrency = Process.GetCurrentProcess().Threads.Count;
        SemaphoreSlim semaphore = new(hardwareConcurrency);
        totalActions = moduleTasks.Sum(p => p.NeedCompileTasks.Length + 1);
        log = totalActions switch
        {
            >= 0 and < 10 => 1,
            >= 10 and < 100 => 2,
            >= 100 and < 1000 => 3,
            >= 1000 and < 10000 => 4,
            >= 10000 and < 100000 => 5,
            _ => 6
        };

        if (ConsoleEnvironment.IsDynamic)
        {
            await AnsiConsole.Progress()
                .Columns([new SpinnerColumn(), new TaskDescriptionColumn(), new ProgressBarColumn(), new PercentageColumn()])
                .StartAsync(async ctx =>
                {
                    Dictionary<GroupDescriptor, ProgressTask> compilationTasks = [];

                    foreach (var result in moduleTasks.GroupBy(p => p.Group))
                    {
                        var task = ctx.AddTask($"Compile {result.Key.Name}", maxValue: result.Sum(p => p.NeedCompileTasks.Length + 1));
                        compilationTasks.Add(result.Key, task);
                    }

                    DispatchCompileWorkers(PulseCounter);
                    DispatchLinkWorkers(PulseCounter);

                    await Task.WhenAll(moduleTasks.Select(p => p.Task));
                    return;

                    void PulseCounter(ITask task)
                    {
                        lock (compilationTasks)
                        {
                            compilationTasks[task.Group].Increment(1);
                            ctx.Refresh();
                        }
                    }
                });
        }
        else
        {
            DispatchCompileWorkers(null);
            DispatchLinkWorkers(null);
            await Task.WhenAll(moduleTasks.Select(p => p.Task));
        }

        return;

        string MakeOutputPrefix()
        {
            return string.Format($"[{{0,{log}}}/{{1,{log}}}]", Interlocked.Increment(ref compiled), totalActions);
        }

        void DispatchLinkWorkers(Action<ITask>? pulse)
        {
            foreach (var moduleTask in moduleTasks)
            {
                moduleTask.LinkAsync(semaphore, moduleTasks, installation, buildTarget, cancellationToken).ContinueWith(r =>
                {
                    try
                    {
                        var output = r.Result;
                        if (ConsoleEnvironment.IsDynamic)
                        {
                            lock (moduleTasks)
                            {
                                for (int i = 0; i < output.Logs.Length; i++)
                                {
                                    Terminal.Log log = output.Logs[i];
                                    if (i == 0)
                                    {
                                        AnsiConsole.MarkupLine("{0} {1}", MakeOutputPrefix().EscapeMarkup(), Terminal.SimpleMarkupOutputLine(log.Value));
                                    }
                                    else
                                    {
                                        AnsiConsole.MarkupLine(Terminal.SimpleMarkupOutputLine(log.Value));
                                    }
                                }
                            }
                        }
                        else
                        {
                            Console.WriteLine("{0} {1}", MakeOutputPrefix(), string.Join('\n', output.Logs.Select(p => p.Value)));
                        }
                    }
                    finally
                    {
                        pulse?.Invoke(moduleTask);
                    }
                });
            }
        }

        void DispatchCompileWorkers(Action<ITask>? pulse)
        {
            var allCompiles = moduleTasks.SelectMany(p => p.NeedCompileTasks).ToArray();
            foreach (var compileTask in allCompiles)
            {
                compileTask.CompileAsync(semaphore, installation, buildTarget, cancellationToken).ContinueWith(r =>
                {
                    try
                    {
                        var output = r.Result;
                        string fileText = string.Format("{0} {1}", MakeOutputPrefix(), compileTask.Item.SourceCode.FilePath);
                        if (ConsoleEnvironment.IsDynamic)
                        {
                            lock (moduleTasks)
                            {
                                AnsiConsole.WriteLine(fileText);
                                foreach (var log in output.Logs.Skip(1))
                                {
                                    AnsiConsole.MarkupLine(Terminal.SimpleMarkupOutputLine(log.Value));
                                }
                            }
                        }
                        else
                        {
                            string[] outputs = new string[output.Logs.Length];
                            outputs[0] = fileText;
                            for (int i = 1; i < output.Logs.Length; ++i)
                            {
                                outputs[i] = output.Logs[i].Value;
                            }
                            Console.WriteLine(string.Join('\n', outputs));
                        }
                    }
                    finally
                    {
                        pulse?.Invoke(compileTask);
                    }
                });
            }
        }
    }
}
