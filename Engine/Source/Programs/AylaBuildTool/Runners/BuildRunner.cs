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
        IEnumerable<ModuleProject> targetProjects;
        if (string.IsNullOrEmpty(options.Target))
        {
            targetProjects = solution.AllProjects.OfType<ModuleProject>();
        }
        else
        {
            var targetProject = (ModuleProject?)solution.FindProject(options.Target);
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

            var resolver = new ModuleRulesResolver(buildTarget, solution, ModuleRules.New(mp.RuleType, buildTarget), mp.Descriptor);
            var depends = solution.FindDepends(resolver.DependencyModuleNames).OfType<ModuleProject>();
            targetProjects = depends.Append(targetProject);
        }

        var installation = new VisualStudioInstallation();
        int compiled = 0;
        int totalActions = 0;
        int log = 1;

        Dictionary<ModuleProject, List<SourceCodeDescriptor>> generatedSourceCodes = [];
        await DispatchGenerateHeaderWorkers();

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

            await GenerateRunner.RunAsync(new GenerateOptions
            {
                ProjectFile = options.ProjectFile
            }, cancellationToken);
            return;
        }

        List<ModuleTask> moduleTasks = [];

        foreach (var project in targetProjects)
        {
            var resolver = new ModuleRulesResolver(buildTarget, solution, ModuleRules.New(project.RuleType, buildTarget), project.Descriptor);
            List<CompileItem> allCompiles = [];
            List<CompileTask> needCompiles = [];
            var intDir = resolver.Group.Intermediate(resolver.Name, buildTarget, FolderPolicy.PathType.Current);

            foreach (var sourceCode in project.GetSourceCodes().Concat(generatedSourceCodes.GetValueOrDefault(project, [])))
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
        const int LinkTasks = 1;
        totalActions = moduleTasks.Sum(p => p.NeedCompileTasks.Length + LinkTasks);
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

        async Task DispatchGenerateHeaderWorkers()
        {
            List<Task<GenerateReflectionHeaderTask>> tasks = [];

            foreach (var project in targetProjects)
            {
                foreach (var sourceCode in project.GetSourceCodes())
                {
                    if (sourceCode.Type == SourceCodeType.Header)
                    {
                        var ght = new GenerateReflectionHeaderTask(project, sourceCode);
                        tasks.Add(ght.GenerateAsync(buildTarget, cancellationToken));
                    }
                }
            }

            var results = await Task.WhenAll(tasks);

            if (results.Any(p => p.ErrorText != null))
            {
                var errorMessages = results.Where(p => p.ErrorText != null).Select(p => p.ErrorText);

                if (ConsoleEnvironment.IsDynamic)
                {
                    AnsiConsole.MarkupLine("[red]{0}[/]", string.Join('\n', errorMessages).EscapeMarkup());
                }
                else
                {
                    Console.WriteLine(string.Join('\n', errorMessages));
                }

                throw TerminateException.User();
            }

            Dictionary<ModuleProject, List<string>> generatedBindingsCodes = [];
            foreach (var result in results)
            {
                var gsc = result.GeneratedSourceCode;
                if (gsc.HasValue)
                {
                    if (generatedSourceCodes.TryGetValue(result.Project, out var list) == false)
                    {
                        list = [];
                        generatedSourceCodes.Add(result.Project, list);
                    }

                    list.Add(gsc.Value);
                }

                var gbc = result.GeneratedBindingCode;
                if (gbc != null)
                {
                    if (generatedBindingsCodes.TryGetValue(result.Project, out var list) == false)
                    {
                        list = [];
                        generatedBindingsCodes.Add(result.Project, list);
                    }

                    list.Add(gbc);
                }
            }

            List<Task> publishBindingsTasks = [];
            foreach (var (project, list) in generatedBindingsCodes)
            {
                bool isNewer = list.Any(p =>
                {
                    string cacheFileName = p + ".cache";
                    if (File.Exists(cacheFileName) == false)
                    {
                        return true;
                    }

                    return SourceCodeCache.LoadCached(cacheFileName).IsModified(SourceCodeCache.MakeCachedSimple(p, project.RuleFilePath));
                });

                if (isNewer || true)
                {
                    var outputPath = project.Descriptor.Output(buildTarget, FolderPolicy.PathType.Current);
                    var assemblyName = $"{project.Name}.Bindings";
                    var dllName = assemblyName + ".dll";
                    var projectFile = Path.Combine(project.Descriptor.Intermediate(project.Name, buildTarget, FolderPolicy.PathType.Current), "Script", project.Name + ".Binding.csproj");
                    await TextFileHelper.WriteIfChangedAsync(projectFile, CSGenerator.GenerateModule(solution, project, buildTarget), cancellationToken);
                    publishBindingsTasks.Add(Terminal.ExecuteCommandAsync($"publish -c {VSUtility.GetCppConfigName(buildTarget)} -o {outputPath}", new Options
                    {
                        Executable = "dotnet",
                        WorkingDirectory = Path.GetDirectoryName(projectFile)!,
                    }, cancellationToken).AsTask().ContinueWith(r =>
                    {
                        r.GetAwaiter().GetResult();
                        foreach (var bindingsCode in list)
                        {
                            string cacheFileName = bindingsCode + ".cache";
                            var cache = SourceCodeCache.MakeCachedSimple(bindingsCode, project.RuleFilePath);
                            cache.SaveCached(cacheFileName);
                        }
                    }));
                }
            }

            await Task.WhenAll(publishBindingsTasks);

            if (ConsoleEnvironment.IsDynamic)
            {
                AnsiConsole.MarkupLine("Reflection header files has been generated.");
            }
            else
            {
                Console.WriteLine("Reflection header files has been generated.");
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
