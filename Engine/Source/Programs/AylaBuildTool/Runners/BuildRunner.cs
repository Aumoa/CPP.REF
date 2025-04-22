using static AylaEngine.CppCompiler;

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
                Console.Error.WriteLine("Target '{0}' is not found in solution.", options.Target);
                throw TerminateException.User();
            }

            if (targetProject is not ModuleProject mp)
            {
                Console.Error.WriteLine("Target '{0}' is not module project. Non module project must be build with specified build program.", options.Target);
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
                if (Directory.Exists(intDir))
                {
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
                }

                var bindingsDir = Path.Combine(intDir, "Bindings");
                if (Directory.Exists(bindingsDir))
                {
                    Directory.Delete(bindingsDir, true);
                }
            }

            var engineOutput = solution.EngineGroup.Output(buildTarget, FolderPolicy.PathType.Current);
            Directory.Delete(engineOutput, true);

            if (solution.PrimaryGroup != null && solution.PrimaryGroup != solution.EngineGroup)
            {
                var primaryOutput = solution.PrimaryGroup.Output(buildTarget, FolderPolicy.PathType.Current);
                Directory.Delete(primaryOutput, true);
            }

            await DispatchGenerateHeaderWorkers();
            await GenerateRunner.RunAsync(new GenerateOptions
            {
                ProjectFile = options.ProjectFile
            }, cancellationToken);
            return;
        }

        await DispatchGenerateHeaderWorkers();
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

        DispatchCompileWorkers();
        DispatchLinkWorkers();
        await Task.WhenAll(moduleTasks.Select(p => p.Task));

        return;

        string MakeOutputPrefix()
        {
            return string.Format($"[{{0,{log}}}/{{1,{log}}}]", Interlocked.Increment(ref compiled), totalActions);
        }

        async Task DispatchGenerateHeaderWorkers()
        {
            Console.Write("Generating reflection header files...");

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
                Console.Error.WriteLine(string.Join('\n', errorMessages));
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

            HashSet<ModuleProject> scriptableProjects = [];
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
                    scriptableProjects.Add(result.Project);
                }
                else if (result.Project.GetRule(buildTarget).EnableScript)
                {
                    scriptableProjects.Add(result.Project);
                }
            }

            Console.WriteLine(" Done.");

            Dictionary<string, TaskCompletionSource> publishBindingsTasks = [];
            foreach (var project in scriptableProjects)
            {
                var rule = project.GetRule(buildTarget);
                if (rule.DisableGenerateBindings == false || rule.EnableScript)
                {
                    publishBindingsTasks.Add(project.Name, new TaskCompletionSource());
                }
            }

            if (scriptableProjects.Count > 0)
            {
                Console.WriteLine("Compiling scripting projects...");
            }

            foreach (var project in scriptableProjects)
            {
                var rule = project.GetRule(buildTarget);

                var sourceDirectory = Path.Combine(project.Group.Intermediate(project.Name, buildTarget, FolderPolicy.PathType.Current), "Bindings");

                var outputPath = project.Group.Output(buildTarget, FolderPolicy.PathType.Current);
                var assemblyName = $"{project.Name}.Bindings";
                var dllName = assemblyName + ".dll";

                var csproj = CSGenerator.GenerateModule(solution, project, false, buildTarget);
                if (DotNETCompiler.NeedCompile(sourceDirectory, assemblyName, project.Group, buildTarget))
                {
                    _ = PublishAsync().ContinueWith(r =>
                    {
                        try
                        {
                            r.GetAwaiter().GetResult();

                            lock (publishBindingsTasks)
                            {
                                publishBindingsTasks[project.Name].SetResult();
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

                    async Task PublishAsync()
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
                        var outputDll = await compiler.CompileAsync(sourceDirectory, assemblyName, csproj, project.Group, buildTarget, cancellationToken);
                        Console.WriteLine("{0} -> {1}.", assemblyName, outputDll);

                        if (rule.EnableScript)
                        {
                            compiler = new DotNETCompiler();
                            sourceDirectory = project.ScriptSourceDirectory;
                            assemblyName = project.ScriptAssemblyName;
                            if (DotNETCompiler.NeedCompile(sourceDirectory, assemblyName, project.Group, buildTarget))
                            {
                                var csproj = CSGenerator.GenerateModule(solution, project, true, buildTarget);
                                outputDll = await compiler.CompileAsync(sourceDirectory, assemblyName, csproj, project.Group, buildTarget, cancellationToken);
                                Console.WriteLine("{0} -> {1}.", assemblyName, outputDll);
                            }
                        }
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

            Console.WriteLine("Reflection header files and scripting DLLs has been generated.");
        }

        void DispatchLinkWorkers()
        {
            foreach (var moduleTask in moduleTasks)
            {
                if (moduleTask.NeedLink(buildTarget))
                {
                    moduleTask.LinkAsync(moduleTasks, installation, buildTarget, cancellationToken).ContinueWith(r =>
                    {
                        var output = r.Result;
                        Console.WriteLine("{0} {1}", MakeOutputPrefix(), string.Join('\n', output.Logs.Select(p => p.Value)));
                    });
                }
                else
                {
                    moduleTask.SetComplete();
                }
            }
        }

        void DispatchCompileWorkers()
        {
            var allCompiles = moduleTasks.SelectMany(p => p.NeedCompileTasks).ToArray();
            foreach (var compileTask in allCompiles)
            {
                compileTask.CompileAsync(installation, buildTarget, cancellationToken).ContinueWith(r =>
                {
                    var output = r.Result;
                    string fileText = string.Format("{0} {1}", MakeOutputPrefix(), compileTask.Item.SourceCode.FilePath);

                    string[] outputs = new string[output.Logs.Length];
                    outputs[0] = fileText;
                    for (int i = 1; i < output.Logs.Length; ++i)
                    {
                        outputs[i] = output.Logs[i].Value;
                    }
                    Console.WriteLine(string.Join('\n', outputs));
                });
            }
        }
    }
}
