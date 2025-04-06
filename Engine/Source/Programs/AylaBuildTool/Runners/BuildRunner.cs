using System;
using System.Diagnostics;
using System.Threading.Tasks;
using Spectre.Console;
using static AylaEngine.Compiler;
using static AylaEngine.Terminal;

namespace AylaEngine;

internal static partial class BuildRunner
{
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
            List<GenerateReflectionHeaderTask> reflectionHeaderGenerates = [];
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
                else if (sourceCode.Type == SourceCodeType.Header)
                {
                    reflectionHeaderGenerates.Add(new GenerateReflectionHeaderTask(resolver, sourceCode));
                }
            }

            moduleTasks.Add(new ModuleTask(resolver, allCompiles.ToArray(), needCompiles.ToArray(), reflectionHeaderGenerates.ToArray()));
        }

        int hardwareConcurrency = Process.GetCurrentProcess().Threads.Count;
        SemaphoreSlim semaphore = new(hardwareConcurrency);
        const int LinkTasks = 1;
        const int GenerateHeaderTasks = 1;
        totalActions = moduleTasks.Sum(p => p.NeedCompileTasks.Length + LinkTasks) + GenerateHeaderTasks;
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

                    await DispatchGenerateHeaderWorkers(PulseCounter);
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
            await DispatchGenerateHeaderWorkers(null);
            DispatchCompileWorkers(null);
            DispatchLinkWorkers(null);
            await Task.WhenAll(moduleTasks.Select(p => p.Task));
        }

        return;

        string MakeOutputPrefix()
        {
            return string.Format($"[{{0,{log}}}/{{1,{log}}}]", Interlocked.Increment(ref compiled), totalActions);
        }

        async Task DispatchGenerateHeaderWorkers(Action<ITask>? pulse)
        {
            List<Task<string>> tasks = [];

            foreach (var moduleTask in moduleTasks)
            {
                foreach (var task in moduleTask.GRHTasks)
                {
                    tasks.Add(task.GenerateAsync(semaphore, buildTarget, cancellationToken));
                }
            }

            string[] results = (await Task.WhenAll(tasks)).Where(r => string.IsNullOrEmpty(r) == false).ToArray();
            if (results.Length > 0)
            {
                if (ConsoleEnvironment.IsDynamic)
                {
                    AnsiConsole.MarkupLine("[red]{0}[/]", string.Join('\n', results).EscapeMarkup());
                }
                else
                {
                    Console.WriteLine(string.Join('\n', results));
                }

                throw TerminateException.User();
            }

            if (ConsoleEnvironment.IsDynamic)
            {
                AnsiConsole.MarkupLine("{0} Reflection header files has been generated.", MakeOutputPrefix().EscapeMarkup());
            }
            else
            {
                Console.WriteLine("{0} Reflection header files has been generated.", MakeOutputPrefix());
            }
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
