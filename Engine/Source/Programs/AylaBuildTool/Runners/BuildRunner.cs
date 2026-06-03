using AylaEngine.RHT;
using static AylaEngine.CppCompiler;

namespace AylaEngine;

internal static partial class BuildRunner
{
    public static async ValueTask RunAsync(BuildOptions options, CancellationToken cancellationToken)
    {
        var buildTarget = BuildRulesExceptionHandler.Evaluate(
            "Failed to create build target information.",
            () => TargetInfo.CreateDefaultTargetInfo(options.Config, options.Editor));
        var solution = await SolutionLoader.CreateDefault().LoadAsync(Global.EngineDirectory, options.ProjectFile, cancellationToken);
        var resolverFactory = new ModuleRulesResolverFactory(solution);
        var scriptProjectFactory = new ScriptProjectFactory(solution, resolverFactory);
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

            List<string> requiredProjects = [];
            var resolver = resolverFactory.GetResolver(mp, buildTarget);
            if (mp.GetRule(buildTarget).Type == ModuleType.Game)
            {
                requiredProjects.Add("Engine");
                requiredProjects.Add("Launch");
                requiredProjects.Add("Direct3D12");
                requiredProjects.Add("WindowsAPI");
            }
            else if (mp.GetRule(buildTarget).Type == ModuleType.Application)
            {
                // Application type (e.g., Launch) requires ApplicationCore and rendering APIs
                // Dependencies are specified in the module's rule file
            }
            else if (mp.GetRule(buildTarget).Type == ModuleType.Console)
            {
                // Console applications don't require ApplicationCore, Launch, or rendering APIs
                // They only need Core and platform-specific dependencies (handled by WithBuiltInDependencyModule)
            }

            var depends = solution.FindDepends(resolver.DependencyModuleNames.Concat(requiredProjects)).OfType<ModuleProject>();
            targetProjects = depends.Append(targetProject).Distinct();
        }

        var installation = Installation.CreateDefaultInstallation();
        int compiled = 0;
        int totalActions = 0;
        int log = 1;
        HashSet<string> loggedTerminalFailures = [];

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
                        if (sourceCode.Type is SourceCodeType.SourceCode or SourceCodeType.ModuleInterface)
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
            if (Directory.Exists(engineOutput))
            {
                Directory.Delete(engineOutput, true);
            }

            if (solution.PrimaryGroup != null && solution.PrimaryGroup != solution.EngineGroup)
            {
                var primaryOutput = solution.PrimaryGroup.Output(buildTarget, FolderPolicy.PathType.Current);
                if (Directory.Exists(primaryOutput))
                {
                    Directory.Delete(primaryOutput, true);
                }
            }
        }

        if (options.Clean is CleanOptions.GenerateOnly or CleanOptions.CleanOnly)
        {
            await DispatchGenerateHeaderWorkers();
            await GenerateRunner.RunAsync(new GenerateOptions
            {
                ProjectFile = options.ProjectFile,
                GeneratorType = options.GeneratorType
            }, cancellationToken);
            return;
        }

        await DispatchGenerateHeaderWorkers();
        Dictionary<ModuleProject, int> buildGraph = [];

        List<ModuleTask> moduleTasks = [];
        List<ScriptTask> scriptTasks = [];
        List<ShaderCompileTask> shaderTasks = [];

        foreach (var project in targetProjects)
        {
            var resolver = resolverFactory.GetResolver(project, buildTarget);
            List<CompileItem> allCompiles = [];
            List<CompileTask> needCompiles = [];

            if (project.GetRule(buildTarget).Type != ModuleType.ThirdParty)
            {
                var intDir = resolver.Group.Intermediate(resolver.Name, buildTarget, FolderPolicy.PathType.Current);

                foreach (var sourceCode in project.GetSourceCodes().Concat(generatedSourceCodes.GetValueOrDefault(project, [])))
                {
                    if (sourceCode.Type is SourceCodeType.SourceCode or SourceCodeType.ModuleInterface)
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
                            var cached = await SourceCodeCache.MakeCachedAsync(installation, item.SourceCode.FilePath, project.RuleFilePath, depsFileName, resolver.DependRuleFilePaths, cancellationToken);
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

                // Collect HLSL shader files for compilation
                var shaderFiles = project.GetSourceCodes()
                    .Where(sc => sc.Type == SourceCodeType.HLSLShader)
                    .ToArray();

                if (shaderFiles.Any())
                {
                    shaderTasks.Add(new ShaderCompileTask(project, buildTarget, shaderFiles, solution.EngineGroup, resolverFactory));
                }
            }

            moduleTasks.Add(new ModuleTask(installation, resolver, allCompiles.ToArray(), needCompiles.ToArray()));
            if (resolver.Rules.Script.Enabled)
            {
                var scriptTask = new ScriptTask(resolver, scriptProjectFactory.GetScriptProject(project));
                if (scriptTask.NeedBuild(buildTarget))
                {
                    scriptTasks.Add(scriptTask);
                }
            }
        }

        totalActions = moduleTasks.Sum(p => p.NeedCompileTasks.Length) + moduleTasks.Count(p => p.NeedLink(buildTarget)) + scriptTasks.Count() + shaderTasks.Count();
        log = totalActions switch
        {
            >= 0 and < 10 => 1,
            >= 10 and < 100 => 2,
            >= 100 and < 1000 => 3,
            >= 1000 and < 10000 => 4,
            >= 10000 and < 100000 => 5,
            _ => 6
        };

        await EnsureShaderCompileWorkerAsync();

        // Execute CMake builds for third-party modules
        await ExecuteCMakeBuilds();

        DispatchCompileWorkers();
        DispatchShaderCompileWorkers();
        DispatchLinkWorkers();
        DispatchScriptCompileWorkers();
        var buildTasks = moduleTasks.Select(p => p.Task).Concat(scriptTasks.Select(p => p.Task)).Concat(shaderTasks.Select(p => p.Task)).ToArray();
        try
        {
            await Task.WhenAll(buildTasks);
        }
        catch
        {
            foreach (var task in buildTasks.Where(p => p.IsFaulted))
            {
                LogTaskFailure(task.Exception);
            }

            throw TerminateException.User();
        }

        return;

        string MakeOutputPrefix(double elapsedSeconds)
        {
            return string.Format($"[{{0,{log}}}/{{1,{log}}} {{2,5:F1}}s]", Interlocked.Increment(ref compiled), totalActions, elapsedSeconds);
        }

        void LogTaskFailure(Exception? exception)
        {
            if (exception == null)
            {
                return;
            }

            IEnumerable<Exception> exceptions = exception is AggregateException ae
                ? ae.Flatten().InnerExceptions
                : [exception];

            foreach (var ex in exceptions)
            {
                if (ex is TerminalExecutionException terminalException)
                {
                    var output = terminalException.Output;
                    var key = $"{output.Executable}\n{output.Command}\n{output.ExitCode}";
                    lock (loggedTerminalFailures)
                    {
                        if (!loggedTerminalFailures.Add(key))
                        {
                            continue;
                        }
                    }

                    var logText = string.Join('\n', output.Logs.Select(l => l.Value));
                    Console.Error.WriteLine("{0} Terminal execution failed with code {1}\n{2}{3}",
                        MakeOutputPrefix(output.ElapsedSeconds),
                        output.ExitCode,
                        output.Command,
                        string.IsNullOrWhiteSpace(logText) ? string.Empty : "\n" + logText);
                }
                else if (ex is not OperationCanceledException)
                {
                    Console.Error.WriteLine(ex.Message);
                }
            }
        }

        async Task EnsureShaderCompileWorkerAsync()
        {
            if (!shaderTasks.Any())
            {
                return;
            }

            var workerTargetInfo = new TargetInfo { Platform = buildTarget.Platform, Config = Configuration.Development, Editor = false };
            var workerPath = Path.Combine(solution.EngineGroup.Output(workerTargetInfo, FolderPolicy.PathType.Current), PlatformUtility.GetExecutableFileName("ShaderCompileWorker"));

            Console.WriteLine("Building ShaderCompileWorker (Development)...");
            await RunAsync(new BuildOptions
            {
                ProjectFile = options.ProjectFile,
                Target = "ShaderCompileWorker",
                Config = Configuration.Development,
                Editor = false,
                GeneratorType = options.GeneratorType,
            }, cancellationToken);

            if (!File.Exists(workerPath))
            {
                Console.Error.WriteLine("Error: Shader compile worker executable not found at {0}", workerPath);
                throw TerminateException.Abort();
            }
        }

        async Task ExecuteCMakeBuilds()
        {
            bool hasFailure = false;

            foreach (var module in moduleTasks)
            {
                try
                {
                    await module.BuildCMakeAsync(buildTarget, cancellationToken);
                }
                catch (TerminalExecutionException)
                {
                    hasFailure = true;
                }
            }

            if (hasFailure)
            {
                throw TerminateException.User();
            }
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
                        var ght = new GenerateReflectionHeaderTask(project, buildTarget, sourceCode);
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

            TypeNames collection;
            {
                Dictionary<string, List<RHTGenerator>> dict = [];
                List<TypeName> typeNames = new();
                foreach (var result in results)
                {
                    if (result.Generator != null)
                    {
                        foreach (var @class in result.Generator.Classes)
                        {
                            typeNames.Add(new ClassName(
                                new NamespaceName(@class.Class.Namespaces.Select(p => p.Name).ToArray()),
                                @class.Class.Name,
                                result.Generator
                            ));
                        }

                        foreach (var @enum in result.Generator.Enums)
                        {
                            typeNames.Add(new EnumName(
                                new NamespaceName(@enum.Namespaces.Select(p => p.Name).ToArray()),
                                @enum.Name,
                                result.Generator
                            ));
                        }
                    }
                }

                collection = new TypeNames(typeNames.ToArray());
            }

            foreach (var result in results)
            {
                if (await result.TryGenerateAsync(collection, buildTarget, cancellationToken) == false)
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
            }

            Console.WriteLine(" Done.");
        }

        void DispatchScriptCompileWorkers()
        {
            Dictionary<string, CSProject> virtualProjects = solution.Projects
                .OfType<ModuleProject>()
                .ToDictionary(p => p.ScriptProjectFileName, p => scriptProjectFactory.GetScriptProject(p));

            foreach (var scriptTask in scriptTasks)
            {
                scriptTask.BuildAsync(scriptTasks, virtualProjects, buildTarget, cancellationToken).ContinueWith(r =>
                {
                    if (r.IsFaulted)
                    {
                        return;
                    }

                    var output = r.Result;
                    Console.WriteLine("{0} {1}", MakeOutputPrefix(output.ElapsedSeconds), string.Join('\n', output.Logs.Select(p => p.Value)));
                });
            }
        }

        void DispatchShaderCompileWorkers()
        {
            foreach (var shaderTask in shaderTasks)
            {
                shaderTask.CompileAsync(moduleTasks, installation, cancellationToken).ContinueWith(r =>
                {
                    if (r.IsFaulted)
                    {
                        return;
                    }

                    var output = r.Result;
                    if (output.Logs.Any())
                    {
                        Console.WriteLine("{0} Compiling shaders for {1}", MakeOutputPrefix(output.ElapsedSeconds), shaderTask.Group.Name);
                    }
                });
            }
        }

        void DispatchLinkWorkers()
        {
            foreach (var moduleTask in moduleTasks)
            {
                if (moduleTask.NeedLink(buildTarget))
                {
                    moduleTask.LinkAsync(moduleTasks, installation, buildTarget, cancellationToken).ContinueWith(r =>
                    {
                        if (r.IsFaulted)
                        {
                            return;
                        }

                        var output = r.Result;
                        Console.WriteLine("{0} {1}", MakeOutputPrefix(output.ElapsedSeconds), string.Join('\n', output.Logs.Select(p => p.Value)));
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
                    if (r.IsFaulted)
                    {
                        return;
                    }

                    var output = r.Result;
                    string fileText = string.Format("{0} {1}", MakeOutputPrefix(output.ElapsedSeconds), compileTask.Item.SourceCode.FilePath);
                    string[] outputs = [fileText, .. output.Logs.Select(l => l.Value)];
                    Console.WriteLine(string.Join('\n', outputs));
                });
            }
        }
    }
}
