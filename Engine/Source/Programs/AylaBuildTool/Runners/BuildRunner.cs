using AylaEngine.RHT;

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
        CppCompiler? compiler = null;

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

                    var resolver = resolverFactory.GetResolver(project, buildTarget);
                    var compileEnvironment = new CppCompileEnvironment(resolver, buildTarget, project.Group);
                    DeletePchFiles(await GetCompilerAsync(), compileEnvironment.PchSettings);
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
        int skippedShaderFileCount = 0;

        foreach (var project in targetProjects)
        {
            var resolver = resolverFactory.GetResolver(project, buildTarget);
            var compileEnvironment = new CppCompileEnvironment(resolver, buildTarget, project.Group);
            List<CppCompileCommand> allCompiles = [];
            List<CompileTask> needCompiles = [];

            if (project.GetRule(buildTarget).Type != ModuleType.ThirdParty)
            {
                CompileTask? pchCompileTask = null;
                if (compileEnvironment.PchSettings != null)
                {
                    await compileEnvironment.PchSettings.WriteSourceFileAsync(cancellationToken);

                    var pchCommand = CppCompileCommand.CreatePch(compileEnvironment);
                    if ((await GetCompilerAsync()).GetCompileOutputFilePaths(pchCommand).Contains(pchCommand.ObjectFilePath))
                    {
                        allCompiles.Add(pchCommand);
                    }

                    if (await NeedCompileAsync(pchCommand))
                    {
                        pchCompileTask = new CompileTask(pchCommand);
                        needCompiles.Add(pchCompileTask);
                    }
                }

                Task? previousPchConsumerTask = pchCompileTask?.Task;
                foreach (var sourceCode in project.GetSourceCodes().Concat(generatedSourceCodes.GetValueOrDefault(project, [])))
                {
                    if (sourceCode.Type is SourceCodeType.SourceCode or SourceCodeType.ModuleInterface)
                    {
                        var command = new CppCompileCommand(compileEnvironment, sourceCode);
                        allCompiles.Add(command);
                        Task[] prerequisiteTasks = command.UsesPch && previousPchConsumerTask != null
                            ? [previousPchConsumerTask]
                            : [];

                        var pchWillRebuild = command.UsesPch && pchCompileTask != null;
                        if (pchWillRebuild || await NeedCompileAsync(command))
                        {
                            var compileTask = new CompileTask(command, prerequisiteTasks);
                            needCompiles.Add(compileTask);
                            if (command.UsesPch)
                            {
                                previousPchConsumerTask = compileTask.Task;
                            }
                        }
                    }
                }
            }

            if (options.SkipShaders)
            {
                skippedShaderFileCount += project.GetSourceCodes().Count(sc => sc.Type == SourceCodeType.HLSLShader);
            }
            else
            {
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

        if (skippedShaderFileCount > 0)
        {
            Console.WriteLine("Skipped shader compilation for {0} shader file(s).", skippedShaderFileCount);
        }

        await EnsureShaderCompileWorkerAsync();

        // Execute CMake builds for third-party modules
        await ExecuteCMakeBuilds();

        List<BuildAction> buildActions = [];
        EnqueueCompileActions(buildActions);
        EnqueueShaderCompileActions(buildActions);
        EnqueueLinkActions(buildActions);
        EnqueueScriptCompileActions(buildActions);
        await new BuildActionExecutor(buildActions).ExecuteAsync(cancellationToken);

        return;

        async ValueTask<bool> NeedCompileAsync(CppCompileCommand command)
        {
            if (options.Clean == CleanOptions.Rebuild)
            {
                return true;
            }

            if (command.CreatesPch)
            {
                if ((await GetCompilerAsync()).GetCompileOutputFilePaths(command).Any(filePath => File.Exists(filePath) == false))
                {
                    return true;
                }
            }

            var cached = await SourceCodeCache.MakeCachedAsync(installation, command.SourceCode.FilePath, command.Resolver.RuleFilePath, command.DependenciesFilePath, command.Resolver.DependRuleFilePaths, command.CacheDependencyFilePaths, cancellationToken);
            return File.Exists(command.CacheFilePath) == false ||
                SourceCodeCache.LoadCached(command.CacheFilePath).IsModified(cached);
        }

        async ValueTask<CppCompiler> GetCompilerAsync()
        {
            return compiler ??= await installation.SpawnCompilerAsync(buildTarget, cancellationToken);
        }

        void DeletePchFiles(CppCompiler activeCompiler, CppPchSettings? pchSettings)
        {
            if (pchSettings == null)
            {
                return;
            }

            foreach (var filePath in activeCompiler.GetPchCleanupFilePaths(pchSettings).Distinct())
            {
                File.Delete(filePath);
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

            List<GenerateReflectionHeaderTask> tasks = [];

            foreach (var project in targetProjects)
            {
                foreach (var sourceCode in project.GetSourceCodes())
                {
                    if (sourceCode.Type == SourceCodeType.Header)
                    {
                        var ght = new GenerateReflectionHeaderTask(project, buildTarget, sourceCode);
                        tasks.Add(ght);
                    }
                }
            }

            RHTGenerationResult result;
            try
            {
                result = await ReflectionHeaderGenerator.GenerateAsync(tasks.Select(task => task.Input), cancellationToken);
            }
            catch (Exception e)
            {
                Console.Error.WriteLine(e.Message);
                throw TerminateException.User();
            }

            if (result.HasErrors)
            {
                Console.Error.WriteLine(string.Join('\n', result.ErrorTexts));
                throw TerminateException.User();
            }

            Dictionary<string, GenerateReflectionHeaderTask> taskBySourceFile = new(StringComparer.OrdinalIgnoreCase);
            foreach (var task in tasks)
            {
                taskBySourceFile.Add(task.Input.SourceFile.FilePath, task);
            }

            foreach (var generatedSource in result.GeneratedSources)
            {
                if (taskBySourceFile.TryGetValue(generatedSource.Input.SourceFile.FilePath, out var task) == false)
                {
                    continue;
                }

                var generatedSourceCode = await task.WriteAsync(generatedSource, cancellationToken);
                if (generatedSourceCodes.TryGetValue(task.Project, out var list) == false)
                {
                    list = [];
                    generatedSourceCodes.Add(task.Project, list);
                }

                if (list.Any(sourceCode => string.Equals(sourceCode.FilePath, generatedSourceCode.FilePath, StringComparison.OrdinalIgnoreCase)) == false)
                {
                    list.Add(generatedSourceCode);
                }
            }

            Console.WriteLine(" Done.");
        }

        void EnqueueScriptCompileActions(List<BuildAction> buildActions)
        {
            Dictionary<string, CSProject> virtualProjects = targetProjects
                .OfType<ModuleProject>()
                .Where(p => p.GetRule(buildTarget).Script.Enabled)
                .ToDictionary(p => p.ScriptProjectFileName, p => scriptProjectFactory.GetScriptProject(p));

            Task? previousScriptTask = null;
            foreach (var scriptTask in SortScriptTasksByDependency())
            {
                var prerequisiteTask = previousScriptTask;
                buildActions.Add(new BuildAction(
                    async ct =>
                    {
                        if (prerequisiteTask != null)
                        {
                            await prerequisiteTask;
                        }

                        return await scriptTask.BuildAsync(scriptTasks, virtualProjects, buildTarget, ct);
                    },
                    output => string.Join('\n', output.Logs.Select(p => p.Value))));
                previousScriptTask = scriptTask.Task;
            }

            List<ScriptTask> SortScriptTasksByDependency()
            {
                Dictionary<string, ScriptTask> taskByName = scriptTasks.ToDictionary(p => p.Resolver.Name, StringComparer.OrdinalIgnoreCase);
                Dictionary<ScriptTask, bool> resolvedTasks = [];
                HashSet<ScriptTask> resolvingTasks = [];
                List<ScriptTask> sortedTasks = [];

                foreach (var scriptTask in scriptTasks)
                {
                    Visit(scriptTask);
                }

                return sortedTasks;

                void Visit(ScriptTask scriptTask)
                {
                    if (resolvedTasks.ContainsKey(scriptTask))
                    {
                        return;
                    }

                    if (resolvingTasks.Add(scriptTask) == false)
                    {
                        throw new InvalidOperationException($"Cyclic script dependency detected at '{scriptTask.Resolver.Name}'.");
                    }

                    foreach (var dependencyName in scriptTask.Resolver.DependencyModuleNames)
                    {
                        if (taskByName.TryGetValue(dependencyName, out var dependencyTask))
                        {
                            Visit(dependencyTask);
                        }
                    }

                    resolvingTasks.Remove(scriptTask);
                    resolvedTasks.Add(scriptTask, true);
                    sortedTasks.Add(scriptTask);
                }
            }
        }

        void EnqueueShaderCompileActions(List<BuildAction> buildActions)
        {
            foreach (var shaderTask in shaderTasks)
            {
                buildActions.Add(new BuildAction(
                    ct => shaderTask.CompileAsync(moduleTasks, installation, ct),
                    _ => $"Compiling shaders for {shaderTask.Group.Name}"));
            }
        }

        void EnqueueLinkActions(List<BuildAction> buildActions)
        {
            foreach (var moduleTask in moduleTasks)
            {
                if (moduleTask.NeedLink(buildTarget))
                {
                    buildActions.Add(new BuildAction(
                        ct => moduleTask.LinkAsync(moduleTasks, installation, buildTarget, ct),
                        output => string.Join('\n', output.Logs.Select(p => p.Value))));
                }
                else
                {
                    moduleTask.SetComplete();
                }
            }
        }

        void EnqueueCompileActions(List<BuildAction> buildActions)
        {
            var allCompiles = moduleTasks.SelectMany(p => p.NeedCompileTasks).ToArray();
            foreach (var compileTask in allCompiles)
            {
                buildActions.Add(new BuildAction(
                    ct => compileTask.CompileAsync(installation, buildTarget, ct),
                    output => string.Join('\n', [compileTask.Command.SourceCode.FilePath, .. output.Logs.Select(l => l.Value)])));
            }
        }
    }
}
