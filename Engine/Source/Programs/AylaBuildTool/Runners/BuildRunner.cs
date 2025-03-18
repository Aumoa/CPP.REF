using System.Diagnostics;
using System.Runtime.Loader;
using Spectre.Console;

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
            Config = Configuration.Debug
        };

        var solution = await Solution.ScanProjectsAsync(Global.EngineDirectory, projectPath, cancellationToken);
        List<Compiler.CompileItem> compileItems = [];
        Dictionary<GroupDescriptor, int> compilationTaskCounts = [];
        foreach (var project in solution.AllProjects)
        {
            if (project is ModuleProject mp)
            {
                var resolver = new ModuleRulesResolver(buildTarget, solution, ModuleRules.New(mp.RuleType, new TargetInfo { Platform = PlatformInfo.Win64 }));

                foreach (var sourceCode in mp.GetSourceCodes())
                {
                    if (sourceCode.Type == SourceCodeType.SourceCode)
                    {
                        compileItems.Add(new Compiler.CompileItem
                        {
                            Resolver = resolver,
                            SourceCode = sourceCode,
                            Descriptor = project.Descriptor
                        });

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
            }
        }

        List<Task> tasks = new();
        var installation = new VisualStudioInstallation();
        int hardwareConcurrency = Process.GetCurrentProcess().Threads.Count;
        SemaphoreSlim semaphore = new(hardwareConcurrency);

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
                            await compiler.CompileAsync(item, cancellationToken);
                        }
                        finally
                        {
                            lock (compilationTasks)
                            {
                                compilationTasks[item.Descriptor].Value += 1;
                                ctx.Refresh();
                            }
                            semaphore.Release();
                        }
                    }
                }

                await Task.WhenAll(tasks);
            });
    }
}
