using System;
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
        private readonly TaskCompletionSource<int> m_CompletionSource = new();

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
                m_CompletionSource.SetResult(output.ExitCode);
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

        public Task<int> Task => m_CompletionSource.Task;

        public GroupDescriptor Group => Item.Descriptor;
    }

    private class ModuleTask : ITask
    {
        public readonly ModuleRulesResolver Resolver;
        public readonly CompileTask[] NeedCompileTasks;

        private readonly CompileItem[] m_AllCompiles;
        private readonly TaskCompletionSource<int> m_CompletionSource = new();
        private readonly CompileTask? m_PrecompileTask;

        public ModuleTask(ModuleRulesResolver resolver, CompileItem[] allCompiles, CompileTask[] needCompiles)
        {
            Resolver = resolver;
            NeedCompileTasks = needCompiles;
            m_AllCompiles = allCompiles;
            if (resolver.Name == "Core")
            {
                int indexOf = Array.FindIndex(needCompiles, p => Path.GetFileName(p.Item.SourceCode.FilePath).Equals("CoreMinimal.pch.cpp", StringComparison.CurrentCultureIgnoreCase));
                if (indexOf != -1)
                {
                    m_PrecompileTask = needCompiles[indexOf];
                    NeedCompileTasks = needCompiles
                        .Where((_, index) => index != indexOf)
                        .ToArray();
                }
            }
        }

        public Task<Terminal.Output?> TryPrecompileAsync(SemaphoreSlim access, Installation installation, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            if (m_PrecompileTask == null)
            {
                return System.Threading.Tasks.Task.FromResult<Terminal.Output?>(null);
            }

            return m_PrecompileTask.CompileAsync(access, installation, targetInfo, cancellationToken)!;
        }

        public async Task<Terminal.Output> LinkAsync(SemaphoreSlim access, IList<ModuleTask> moduleTasks, Installation installation, TargetInfo targetInfo, CancellationToken cancellationToken)
        {
            var compileResults = await System.Threading.Tasks.Task.WhenAll(NeedCompileTasks.Select(p => p.Task));
            if (compileResults.Any(p => p != 0))
            {
                var err = new Terminal.Log
                {
                    Value = "Link error: One or more required source code compilations have failed.",
                    Verbosity = Verbose.Error
                };

                m_CompletionSource.SetResult(-1);
                return new Terminal.Output
                {
                    ExitCode = -1,
                    Executable = string.Empty,
                    Command = string.Empty,
                    Logs = [err],
                    StdOut = [],
                    StdErr = [err]
                };
            }

            foreach (var name in Resolver.DependencyModuleNames)
            {
                var dependLinkResult = await moduleTasks.Where(p => p.Resolver.Name == name).First().Task;
                if (dependLinkResult != 0)
                {
                    var err = new Terminal.Log
                    {
                        Value = "Link error: One or more required module linking have failed.",
                        Verbosity = Verbose.Error
                    };

                    m_CompletionSource.SetResult(-1);
                    return new Terminal.Output
                    {
                        ExitCode = -1,
                        Executable = string.Empty,
                        Command = string.Empty,
                        Logs = [err],
                        StdOut = [],
                        StdErr = [err]
                    };
                }
            }

            await access.WaitAsync(cancellationToken);
            try
            {
                var linker = await installation.SpawnLinkerAsync(targetInfo, cancellationToken);
                var output = await linker.LinkAsync(Resolver, m_AllCompiles, cancellationToken);
                m_CompletionSource.SetResult(output.ExitCode);
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

        public Task<int> Task => m_CompletionSource.Task;

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

        if (options.Clean == CleanOptions.CleanOnly)
        {
            foreach (var project in targetProjects.OfType<ModuleProject>())
            {
                var intDir = project.Descriptor.Intermediate(project.Name, buildTarget, FolderPolicy.PathType.Current);
                foreach (var sourceCode in project.GetSourceCodes())
                {
                    var fileName = Path.GetFileName(sourceCode.FilePath);
                    var cacheFileName = Path.Combine(intDir, fileName + ".cache");
                    File.Delete(cacheFileName);
                }
            }

            return;
        }

        List<ModuleTask> moduleTasks = [];

        foreach (var project in targetProjects.OfType<ModuleProject>())
        {
            var resolver = new ModuleRulesResolver(buildTarget, solution, ModuleRules.New(project.RuleType, buildTarget), project.Descriptor, primaryGroup);
            List<CompileItem> allCompiles = [];
            List<CompileTask> needCompiles = [];
            var intDir = resolver.Group.Intermediate(resolver.Name, buildTarget, FolderPolicy.PathType.Current);

            foreach (var sourceCode in project.GetSourceCodes())
            {
                if (sourceCode.Type == SourceCodeType.SourceCode)
                {
                    var item = new Compiler.CompileItem
                    {
                        Resolver = resolver,
                        SourceCode = sourceCode,
                        Descriptor = project.Descriptor
                    };

                    var fileName = Path.GetFileName(item.SourceCode.FilePath);
                    var cacheFileName = Path.Combine(intDir, fileName + ".cache");
                    var depsFileName = Path.Combine(intDir, fileName + ".deps.json");
                    allCompiles.Add(item);

                    if (options.Clean != CleanOptions.Rebuild)
                    {
                        var cached = await SourceCodeCache.MakeCachedAsync(item.SourceCode.FilePath, project.RuleFilePath, depsFileName, resolver.DependRuleFilePaths, cancellationToken);
                        if (File.Exists(cacheFileName) == false ||
                            SourceCodeCache.LoadCached(cacheFileName).IsModified(cached))
                        {
                            needCompiles.Add(new CompileTask(item));
                        }
                    }
                    else
                    {
                        needCompiles.Add(new CompileTask(item));
                    }
                }
            }

            moduleTasks.Add(new ModuleTask(resolver, allCompiles.ToArray(), needCompiles.ToArray()));
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

        int[] buildResults;
        if (ConsoleEnvironment.IsDynamic)
        {
            buildResults = await AnsiConsole.Progress()
                .Columns([new SpinnerColumn(), new TaskDescriptionColumn(), new ProgressBarColumn(), new PercentageColumn()])
                .StartAsync(async ctx =>
                {
                    Dictionary<GroupDescriptor, ProgressTask> compilationTasks = [];

                    foreach (var result in moduleTasks.GroupBy(p => p.Group))
                    {
                        var task = ctx.AddTask($"Compile {result.Key.Name}", maxValue: result.Sum(p => p.NeedCompileTasks.Length + 1));
                        compilationTasks.Add(result.Key, task);
                    }

                    await PrecompileAsync(PulseCounter);
                    DispatchCompileWorkers(PulseCounter);
                    DispatchLinkWorkers(PulseCounter);

                    return await Task.WhenAll(moduleTasks.Select(p => p.Task));

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
            await PrecompileAsync(null);
            DispatchCompileWorkers(null);
            DispatchLinkWorkers(null);
            buildResults = await Task.WhenAll(moduleTasks.Select(p => p.Task));
        }

        if (buildResults.Any(p => p != 0))
        {
            AnsiConsole.MarkupLine("[red]Build failed.[/]");
            throw TerminateException.User();
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

        async Task PrecompileAsync(Action<ITask>? pulse)
        {
            List<Task> tasks = [];

            foreach (var moduleTask in moduleTasks)
            {
                var precompileTask = moduleTask.TryPrecompileAsync(semaphore, installation, buildTarget, cancellationToken);
                tasks.Add(precompileTask);
                _ = precompileTask.ContinueWith(r =>
                {
                    var output = r.Result;
                    if (output == null)
                    {
                        return;
                    }

                    try
                    {
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

            await Task.WhenAll(tasks);
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
