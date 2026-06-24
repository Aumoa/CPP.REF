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
        ModuleProject[] targetProjects = ResolveTargetProjects(buildTarget, options.Target);

        var installation = Installation.CreateDefaultInstallation();
        Dictionary<TargetInfo, CppCompiler> compilerCache = [];
        Dictionary<(TargetInfo TargetInfo, ModuleProject Project), ModuleTask> moduleTaskCache = [];
        Dictionary<(TargetInfo TargetInfo, ModuleProject Project), ScriptTask> scriptTaskCache = [];

        if (options.Clean == CleanOptions.CleanOnly)
        {
            foreach (var project in targetProjects.OfType<ModuleProject>())
            {
                var buildProfile = BuildProfileResolver.Resolve(project, buildTarget);
                var intDir = project.Group.Intermediate(project.Name, buildTarget, buildProfile, FolderPolicy.PathType.Current);
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
                    DeletePchFiles(await GetCompilerAsync(buildTarget), compileEnvironment.PchSettings);
                }

                var bindingsDir = Path.Combine(intDir, "Bindings");
                if (Directory.Exists(bindingsDir))
                {
                    Directory.Delete(bindingsDir, true);
                }
            }

            var engineOutput = solution.EngineGroup.Output(buildTarget, BuildProfileResolver.Resolve(solution.EngineGroup, buildTarget), FolderPolicy.PathType.Current);
            if (Directory.Exists(engineOutput))
            {
                Directory.Delete(engineOutput, true);
            }

            if (solution.PrimaryGroup != null && solution.PrimaryGroup != solution.EngineGroup)
            {
                var primaryOutput = solution.PrimaryGroup.Output(buildTarget, BuildProfileResolver.Resolve(solution.PrimaryGroup, buildTarget), FolderPolicy.PathType.Current);
                if (Directory.Exists(primaryOutput))
                {
                    Directory.Delete(primaryOutput, true);
                }
            }
        }

        if (options.Clean is CleanOptions.GenerateOnly or CleanOptions.CleanOnly)
        {
            await DispatchGenerateHeaderWorkers(targetProjects, buildTarget, []);
            await GenerateRunner.RunAsync(new GenerateOptions
            {
                ProjectFile = options.ProjectFile,
                GeneratorType = options.GeneratorType
            }, cancellationToken);
            return;
        }

        var (primaryPlanTarget, skippedShaderFileCount) = await CreateBuildPlanTargetAsync(targetProjects, buildTarget, options.SkipShaders, includeScripts: true);
        List<BuildPlanTarget> planTargets = [primaryPlanTarget];

        if (skippedShaderFileCount > 0)
        {
            Console.WriteLine("Skipped shader compilation for {0} shader file(s).", skippedShaderFileCount);
        }

        if (primaryPlanTarget.ShaderTasks.Count > 0)
        {
            var workerTargetInfo = new TargetInfo { Platform = buildTarget.Platform, Config = Configuration.Development, Editor = false };
            var workerTargetProjects = ResolveTargetProjects(workerTargetInfo, "ShaderCompileWorker");
            var (workerPlanTarget, _) = await CreateBuildPlanTargetAsync(workerTargetProjects, workerTargetInfo, skipShaders: true, includeScripts: false);
            planTargets.Insert(0, workerPlanTarget);
        }

        // Execute CMake builds for third-party modules
        await ExecuteCMakeBuilds(planTargets);

        var buildPlan = new BuildPlanBuilder(
            planTargets,
            installation,
            scriptProjectFactory).Build();
        await buildPlan.ExecuteAsync(cancellationToken);

        return;

        ModuleProject[] ResolveTargetProjects(TargetInfo targetInfo, string? targetName)
        {
            if (string.IsNullOrEmpty(targetName))
            {
                return solution.Projects.OfType<ModuleProject>().ToArray();
            }

            var foundTargetProject = solution.FindProject(targetName);
            if (foundTargetProject == null)
            {
                Console.Error.WriteLine("Target '{0}' is not found in solution.", targetName);
                throw TerminateException.User();
            }

            if (foundTargetProject is not ModuleProject targetProject)
            {
                Console.Error.WriteLine("Target '{0}' is not module project. Non module project must be build with specified build program.", targetName);
                throw TerminateException.User();
            }

            List<string> requiredProjects = [];
            var resolver = resolverFactory.GetResolver(targetProject, targetInfo);
            if (targetProject.GetRule(targetInfo).Type == ModuleType.Game)
            {
                requiredProjects.Add("Engine");
                requiredProjects.Add("Launch");
                requiredProjects.Add("Direct3D12");
                requiredProjects.Add("WindowsAPI");
            }
            else if (targetProject.GetRule(targetInfo).Type == ModuleType.Application)
            {
                // Application type (e.g., Launch) requires ApplicationCore and rendering APIs.
                // Dependencies are specified in the module's rule file.
            }
            else if (targetProject.GetRule(targetInfo).Type == ModuleType.Console)
            {
                // Console applications use built-in platform dependencies from ModuleRulesResolver.
            }

            var depends = solution.FindDepends(resolver.DependencyModuleNames.Concat(requiredProjects)).OfType<ModuleProject>();
            return depends.Append(targetProject).Distinct().ToArray();
        }

        async Task<(BuildPlanTarget Target, int SkippedShaderFileCount)> CreateBuildPlanTargetAsync(ModuleProject[] projects, TargetInfo targetInfo, bool skipShaders, bool includeScripts)
        {
            Dictionary<ModuleProject, List<SourceCodeDescriptor>> generatedSourceCodes = [];
            await DispatchGenerateHeaderWorkers(projects, targetInfo, generatedSourceCodes);

            List<ModuleTask> moduleTasks = [];
            List<ScriptTask> scriptTasks = [];
            List<ShaderCompileTask> shaderTasks = [];
            int skippedShaderFileCount = 0;

            foreach (var project in projects)
            {
                var resolver = resolverFactory.GetResolver(project, targetInfo);
                var moduleTaskKey = (targetInfo, project);
                if (moduleTaskCache.TryGetValue(moduleTaskKey, out var moduleTask) == false)
                {
                    var compileEnvironment = new CppCompileEnvironment(resolver, targetInfo, project.Group);
                    List<CppCompileCommand> allCompiles = [];
                    List<CompileTask> needCompiles = [];

                    if (project.GetRule(targetInfo).Type != ModuleType.ThirdParty)
                    {
                        CompileTask? pchCompileTask = null;
                        if (compileEnvironment.PchSettings != null)
                        {
                            await compileEnvironment.PchSettings.WriteSourceFileAsync(cancellationToken);

                            var pchCommand = CppCompileCommand.CreatePch(compileEnvironment);
                            if ((await GetCompilerAsync(targetInfo)).GetCompileOutputFilePaths(pchCommand).Contains(pchCommand.ObjectFilePath))
                            {
                                allCompiles.Add(pchCommand);
                            }

                            if (await NeedCompileAsync(pchCommand, targetInfo))
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
                                if (pchWillRebuild || await NeedCompileAsync(command, targetInfo))
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

                    moduleTask = new ModuleTask(installation, resolver, allCompiles.ToArray(), needCompiles.ToArray());
                    moduleTaskCache.Add(moduleTaskKey, moduleTask);
                }

                moduleTasks.Add(moduleTask);

                if (skipShaders)
                {
                    skippedShaderFileCount += project.GetSourceCodes().Count(sc => sc.Type == SourceCodeType.HLSLShader);
                }
                else
                {
                    var shaderFiles = project.GetSourceCodes()
                        .Where(sc => sc.Type == SourceCodeType.HLSLShader)
                        .ToArray();

                    if (shaderFiles.Any())
                    {
                        shaderTasks.Add(new ShaderCompileTask(project, targetInfo, shaderFiles, solution.EngineGroup, resolverFactory));
                    }
                }

                if (includeScripts && resolver.Rules.Script.Enabled)
                {
                    var scriptTaskKey = (targetInfo, project);
                    if (scriptTaskCache.TryGetValue(scriptTaskKey, out var scriptTask) == false)
                    {
                        scriptTask = new ScriptTask(resolver, scriptProjectFactory.GetScriptProject(project));
                        if (scriptTask.NeedBuild(targetInfo))
                        {
                            scriptTaskCache.Add(scriptTaskKey, scriptTask);
                        }
                        else
                        {
                            scriptTask = null;
                        }
                    }

                    if (scriptTask != null)
                    {
                        scriptTasks.Add(scriptTask);
                    }
                }
            }

            return (new BuildPlanTarget(targetInfo, projects, moduleTasks, scriptTasks, shaderTasks), skippedShaderFileCount);
        }

        async ValueTask<bool> NeedCompileAsync(CppCompileCommand command, TargetInfo targetInfo)
        {
            if (options.Clean == CleanOptions.Rebuild)
            {
                return true;
            }

            if (command.CreatesPch)
            {
                if ((await GetCompilerAsync(targetInfo)).GetCompileOutputFilePaths(command).Any(filePath => File.Exists(filePath) == false))
                {
                    return true;
                }
            }

            var cached = await SourceCodeCache.MakeCachedAsync(installation, command.SourceCode.FilePath, command.Resolver.RuleFilePath, command.DependenciesFilePath, command.Resolver.DependRuleFilePaths, command.CacheDependencyFilePaths, cancellationToken);
            return File.Exists(command.CacheFilePath) == false ||
                SourceCodeCache.LoadCached(command.CacheFilePath).IsModified(cached);
        }

        async ValueTask<CppCompiler> GetCompilerAsync(TargetInfo targetInfo)
        {
            if (compilerCache.TryGetValue(targetInfo, out var compiler))
            {
                return compiler;
            }

            compiler = await installation.SpawnCompilerAsync(targetInfo, cancellationToken);
            compilerCache.Add(targetInfo, compiler);
            return compiler;
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

        async Task ExecuteCMakeBuilds(IEnumerable<BuildPlanTarget> targets)
        {
            bool hasFailure = false;
            HashSet<(TargetInfo TargetInfo, ModuleTask ModuleTask)> visitedTasks = [];

            foreach (var target in targets)
            {
                foreach (var module in target.ModuleTasks)
                {
                    if (visitedTasks.Add((target.TargetInfo, module)) == false)
                    {
                        continue;
                    }

                    try
                    {
                        await module.BuildCMakeAsync(target.TargetInfo, cancellationToken);
                    }
                    catch (TerminalExecutionException)
                    {
                        hasFailure = true;
                    }
                }
            }

            if (hasFailure)
            {
                throw TerminateException.User();
            }
        }

        async Task DispatchGenerateHeaderWorkers(ModuleProject[] projects, TargetInfo targetInfo, Dictionary<ModuleProject, List<SourceCodeDescriptor>> generatedSourceCodes)
        {
            Console.Write("Generating reflection header files...");

            List<GenerateReflectionHeaderTask> tasks = [];

            foreach (var project in projects)
            {
                foreach (var sourceCode in project.GetSourceCodes())
                {
                    if (sourceCode.Type == SourceCodeType.Header)
                    {
                        var ght = new GenerateReflectionHeaderTask(project, targetInfo, sourceCode);
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
    }
}
