using System;
using System.Diagnostics;
using System.Threading.Tasks;
using Spectre.Console;
using static AylaEngine.CppCompiler;
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
            Editor = options.Editor,
            Config = options.Config
        };

        var solution = await Solution.ScanProjectsAsync(Global.EngineDirectory, projectPath, cancellationToken);
        Dictionary<GroupDescriptor, int> compilationTaskCounts = [];
        IEnumerable<ModuleProject> targetProjects;
        if (string.IsNullOrEmpty(options.Target))
        {
            targetProjects = solution.Projects.OfType<ModuleProject>();
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

            var resolver = mp.GetResolver(buildTarget);
            var depends = solution.FindDepends(resolver.DependencyModuleNames).OfType<ModuleProject>();
            targetProjects = depends.Append(targetProject);
        }

        var installation = new VisualStudioInstallation();
        int compiled = 0;
        int totalActions = 0;
        int log = 1;

        Dictionary<ModuleProject, List<SourceCodeDescriptor>> generatedSourceCodes = [];

        if (options.Clean == CleanOptions.CleanOnly)
        {
            foreach (var project in targetProjects.OfType<ModuleProject>())
            {
                var intDir = project.Group.Intermediate(project.Name, buildTarget, FolderPolicy.PathType.Current);
                foreach (var sourceCode in project.GetSourceCodes())
                {
                    if (sourceCode.Type == SourceCodeType.SourceCode)
                    {
                        var fileName = Path.GetFileName(sourceCode.FilePath);
                        var cacheFileName = Path.Combine(intDir, fileName + ".cache");
                        File.Delete(cacheFileName);
                        var outputFileName = Path.Combine(intDir, fileName + ".o");
                        File.Delete(outputFileName);
                        var pdbFileName = Path.Combine(intDir, fileName + ".pdb");
                        File.Delete(pdbFileName);
                        var depsFileName = Path.Combine(intDir, fileName + ".deps");
                        File.Delete(depsFileName);
                        fileName = Path.ChangeExtension(fileName, null);
                        var genSourceFileName = Path.Combine(intDir, fileName + ".gen.cpp");
                        File.Delete(genSourceFileName);
                        var genHeaderFileName = Path.Combine(intDir, fileName + ".gen.h");
                        File.Delete(genHeaderFileName);
                        var genDepsFileName = Path.Combine(intDir, fileName + ".gen.cpp.deps");
                        File.Delete(genDepsFileName);
                        var genCacheFileName = Path.Combine(intDir, fileName + ".gen.cpp.cache");
                        File.Delete(genCacheFileName);
                        var genPdbFileName = Path.Combine(intDir, fileName + ".gen.cpp.pdb");
                        File.Delete(genPdbFileName);
                        var genOutputFileName = Path.Combine(intDir, fileName + ".gen.cpp.o");
                        File.Delete(genOutputFileName);
                    }
                }

                var bindingsDir = Path.Combine(intDir, "Bindings");
                if (Directory.Exists(bindingsDir))
                {
                    Directory.Delete(bindingsDir, true);
                }
            }

            await DispatchGenerateHeaderWorkers();
            await GenerateRunner.RunAsync(new GenerateOptions
            {
                ProjectFile = options.ProjectFile
            }, cancellationToken);
            return;
        }

        await DispatchGenerateHeaderWorkers();
        await DispatchScriptBuild();
        List<ModuleTask> moduleTasks = [];

        foreach (var project in targetProjects)
        {
            var resolver = project.GetResolver(buildTarget);
            List<CompileItem> allCompiles = [];
            List<CompileTask> needCompiles = [];
            var intDir = resolver.Group.Intermediate(resolver.Name, buildTarget, FolderPolicy.PathType.Current);

            foreach (var sourceCode in project.GetSourceCodes().Concat(generatedSourceCodes.GetValueOrDefault(project, [])))
            {
                if (sourceCode.Type == SourceCodeType.SourceCode)
                {
                    var item = new CppCompiler.CompileItem
                    {
                        Resolver = resolver,
                        SourceCode = sourceCode,
                        Descriptor = project.Group
                    };

                    var fileName = Path.GetFileName(item.SourceCode.FilePath);
                    var cacheFileName = Path.Combine(intDir, fileName + ".cache");
                    var depsFileName = Path.Combine(intDir, fileName + ".deps");
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

        totalActions = moduleTasks.Sum(p => p.NeedCompileTasks.Length) + moduleTasks.Count(p => p.NeedLink(buildTarget));
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

        async Task DispatchScriptBuild()
        {
            List<Task> tasks = [];

            foreach (var project in targetProjects)
            {
                if (project.GetRule(buildTarget).EnableScript)
                {
                    var scriptProjectFileName = project.GetScriptProjectFileName();
                    var compiler = new DotNETCompiler();
                    if (DotNETCompiler.NeedCompile(scriptProjectFileName, project.Group, buildTarget))
                    {
                        tasks.Add(compiler.CompileAsync(scriptProjectFileName, project.Group, buildTarget, cancellationToken));
                    }
                }
            }

            await Task.WhenAll(tasks);
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
                        tasks.Add(ght.ParseAsync(cancellationToken));
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

            RHTGenerator.Collection generators;
            {
                Dictionary<string, List<RHTGenerator>> dict = [];
                foreach (var result in results)
                {
                    if (result.Generator != null)
                    {
                        foreach (var @class in result.Generator.Classes)
                        {
                            var cname = @class.Class.Name;
                            if (dict.TryGetValue(cname, out var list) == false)
                            {
                                list = new List<RHTGenerator>();
                                dict.Add(cname, list);
                            }

                            list.Add(result.Generator);
                        }
                    }
                }

                generators = new RHTGenerator.Collection(dict);
            }

            Dictionary<ModuleProject, List<string>> generatedBindingsCodes = [];
            foreach (var result in results)
            {
                if (await result.TryGenerateAsync(generators, buildTarget, cancellationToken) == false)
                {
                    continue;
                }

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

            Dictionary<string, TaskCompletionSource> publishBindingsTasks = [];
            foreach (var (project, list) in generatedBindingsCodes)
            {
                var rule = project.GetRule(buildTarget);
                if (rule.DisableGenerateBindings == false)
                {
                    publishBindingsTasks.Add(project.Name, new TaskCompletionSource());
                }
            }

            foreach (var (project, list) in generatedBindingsCodes)
            {
                var projectFile = Path.Combine(project.Group.Intermediate(project.Name, buildTarget, FolderPolicy.PathType.Current), "Bindings", project.Name + ".Bindings.csproj");

                var outputPath = project.Group.Output(buildTarget, FolderPolicy.PathType.Current);
                var assemblyName = $"{project.Name}.Bindings";
                var dllName = assemblyName + ".dll";

                var csproj = CSGenerator.GenerateModule(solution, project, false, buildTarget);
                await TextFileHelper.WriteIfChangedAsync(projectFile, csproj, cancellationToken);

                if (DotNETCompiler.NeedCompile(projectFile, project.Group, buildTarget))
                {
                    _ = PublishAsync().ContinueWith(r =>
                    {
                        try
                        {
                            var output = r.Result;
                            if (output.IsCompletedSuccessfully)
                            {
                                lock (publishBindingsTasks)
                                {
                                    publishBindingsTasks[project.Name].SetResult();
                                }
                            }
                            else
                            {
                                throw TerminateException.User();
                            }
                        }
                        catch (OperationCanceledException)
                        {
                            lock (publishBindingsTasks)
                            {
                                publishBindingsTasks[project.Name].SetCanceled();
                            }
                        }
                        catch (Exception e)
                        {
                            lock (publishBindingsTasks)
                            {
                                publishBindingsTasks[project.Name].SetException(e);
                            }
                        }
                    });

                    async Task<Terminal.Output> PublishAsync()
                    {
                        var resolver = project.GetResolver(buildTarget);
                        foreach (var depend in resolver.DependencyModuleNames)
                        {
                            Task dependTask;
                            lock (publishBindingsTasks)
                            {
                                if (publishBindingsTasks.TryGetValue(depend, out var tcs))
                                {
                                    dependTask = tcs.Task;
                                }
                                else
                                {
                                    dependTask = Task.CompletedTask;
                                }
                            }

                            await dependTask;
                        }

                        Directory.CreateDirectory(outputPath);
                        var compiler = new DotNETCompiler();
                        return await compiler.CompileAsync(projectFile, project.Group, buildTarget, cancellationToken);
                    }
                }
                else
                {
                    lock (publishBindingsTasks)
                    {
                        publishBindingsTasks[project.Name].SetResult();
                    }
                }
            }

            await Task.WhenAll(publishBindingsTasks.Values.Select(p => p.Task));

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
                if (moduleTask.NeedLink(buildTarget))
                {
                    moduleTask.LinkAsync(moduleTasks, installation, buildTarget, cancellationToken).ContinueWith(r =>
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
                else
                {
                    moduleTask.SetComplete();
                }
            }
        }

        void DispatchCompileWorkers(Action<ITask>? pulse)
        {
            var allCompiles = moduleTasks.SelectMany(p => p.NeedCompileTasks).ToArray();
            foreach (var compileTask in allCompiles)
            {
                compileTask.CompileAsync(installation, buildTarget, cancellationToken).ContinueWith(r =>
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
