using System.Diagnostics;
using Spectre.Console;
using static AylaEngine.Compiler;

namespace AylaEngine;

internal static class BuildRunner
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
        List<Compiler.CompileItem> compileItems = [];
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
        List<Func<ValueTask>> queuedLinkerTasks = [];
        int compiled = 0;
        int totalActions = 0;
        int log = 1;

        foreach (var project in targetProjects)
        {
            if (project is ModuleProject mp)
            {
                var resolver = new ModuleRulesResolver(buildTarget, solution, ModuleRules.New(mp.RuleType, buildTarget), mp.Descriptor, primaryGroup);
                List<CompileItem> moduleItems = [];

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
                        if (File.Exists(cacheFileName) == false ||
                            SourceCodeCache.LoadCached(cacheFileName).IsModified(cached))
                        {
                            compileItems.Add(item);
                        }

                        moduleItems.Add(item);

                        if (compilationTaskCounts.TryGetValue(mp.Descriptor, out int count))
                        {
                            compilationTaskCounts[mp.Descriptor] = ++count;
                        }
                        else
                        {
                            compilationTaskCounts[mp.Descriptor] = 1;
                        }
                    }
                }

                queuedLinkerTasks.Add(async () =>
                {
                    var linker = await installation.SpawnLinkerAsync(buildTarget, cancellationToken);
                    var output = await linker.LinkAsync(resolver, moduleItems.ToArray(), cancellationToken);
                    if (ConsoleEnvironment.IsDynamic)
                    {
                        lock (compileItems)
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
                });
            }
        }

        totalActions = compileItems.Count + queuedLinkerTasks.Count;
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
                    foreach (var (descriptor, count) in compilationTaskCounts)
                    {
                        var task = ctx.AddTask($"Compile {descriptor.Name}", maxValue: compileItems.Count);
                        compilationTasks.Add(descriptor, task);
                    }

                    await CompileAsync(item =>
                    {
                        lock (compilationTasks)
                        {
                            compilationTasks[item.Descriptor].Increment(1);
                            ctx.Refresh();
                        }
                    });
                });
        }
        else
        {
            await CompileAsync(null);
        }

        foreach (var queuedLinkerTask in queuedLinkerTasks)
        {
            await queuedLinkerTask();
        }

        return;

        string MakeOutputPrefix()
        {
            return string.Format($"[{{0,{log}}}/{{1,{log}}}]", Interlocked.Increment(ref compiled), totalActions);
        }

        async ValueTask CompileAsync(Action<CompileItem>? pulse)
        {
            int hardwareConcurrency = Process.GetCurrentProcess().Threads.Count;
            SemaphoreSlim semaphore = new(hardwareConcurrency);
            List<Task> tasks = new();
            foreach (var item in compileItems)
            {
                tasks.Add(SingleTask());

                continue;

                async Task SingleTask()
                {
                    await semaphore.WaitAsync(cancellationToken);
                    try
                    {
                        var compiler = await installation.SpawnCompilerAsync(buildTarget, cancellationToken);
                        var output = await compiler.CompileAsync(item, cancellationToken);
                        string fileText = string.Format("{0} {1}", MakeOutputPrefix(), item.SourceCode.FilePath);
                        if (ConsoleEnvironment.IsDynamic)
                        {
                            lock (compileItems)
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
                        pulse?.Invoke(item);
                        semaphore.Release();
                    }
                }
            }

            await Task.WhenAll(tasks);
        }
    }
}
