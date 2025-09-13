using AylaEngine.Installations;
using AylaEngine.RHT;
using AylaEngine.RHT.Types;
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

        var buildTarget = TargetInfo.CreateDefaultTargetInfo(options);
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

            List<string> requiredProjects = [];
            var resolver = mp.GetResolver(buildTarget);
            if (mp.GetRule(buildTarget).Type == ModuleType.Game)
            {
                requiredProjects.Add("Engine");
                requiredProjects.Add("Launch");
                requiredProjects.Add("VulkanAPI");
                requiredProjects.Add("WindowsAPI");
            }

            var depends = solution.FindDepends(resolver.DependencyModuleNames.Concat(requiredProjects)).OfType<ModuleProject>();
            targetProjects = depends.Append(targetProject).Distinct();
        }

        var installation = Installation.CreateDefaultInstallation();
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
        await BuildScriptProjects(buildGraph, targetProjects);

        List<ModuleTask> moduleTasks = [];

        foreach (var project in targetProjects)
        {
            var resolver = project.GetResolver(buildTarget);
            List<CompileItem> allCompiles = [];
            List<CompileTask> needCompiles = [];
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

            moduleTasks.Add(new ModuleTask(installation, resolver, allCompiles.ToArray(), needCompiles.ToArray()));
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

            var projects = results.Where(p => p.Generator != null || p.Project.Name == "Core").GroupBy(p => p.Project);
            List<string> buildTargets = [];
            foreach (var g in projects)
            {
                var project = g.Key;
                var csprojPath = project.ScriptProjectFileName;

                string projectReferences = string.Empty;
                var resolver = project.GetResolver(buildTarget);

                var depends = solution.FindDepends(resolver.DependencyModuleNames).OfType<ModuleProject>().ToArray();
                if (depends.Length > 0)
                {
                    projectReferences += "\n";
                    projectReferences += "  <ItemGroup>\n";
                    foreach (var depend in GetParents(depends.Select(p => p.Name)).Distinct())
                    {
                        projectReferences += $"    <ProjectReference Include=\"{depend.ScriptProjectFileName}\" />\n";
                    }
                    projectReferences += "  </ItemGroup>\n";

                    IEnumerable<ModuleProject> GetParents(IEnumerable<string> current)
                    {
                        foreach (var item in solution.FindDepends(current).OfType<ModuleProject>())
                        {
                            if (projects.Any(p => p.Key == item))
                            {
                                yield return item;
                            }
                            else
                            {
                                var resolver2 = item.GetResolver(buildTarget);
                                foreach (var parent in GetParents(resolver2.DependencyModuleNames))
                                {
                                    yield return parent;
                                }
                            }
                        }
                    }
                }

                string nugetPackages = string.Empty;
                if (resolver.Rules.Scriptable.NuGetPackages.Count > 0)
                {
                    nugetPackages += "\n";
                    nugetPackages += "  <ItemGroup>\n";
                    foreach (var pkg in resolver.Rules.Scriptable.NuGetPackages)
                    {
                        nugetPackages += $"    <PackageReference Include=\"{pkg.Id}\" Version=\"{pkg.Version}\" />\n";
                    }
                    nugetPackages += "  </ItemGroup>\n";
                }

                string namespaceName;
                if (project.Group == solution.EngineGroup)
                {
                    namespaceName = "Ayla";
                }
                else
                {
                    namespaceName = project.Group.Name;
                }

                var platforms = string.Join(';', PlatformInfo.GetAllPlatforms().Select(p => p.Name));
                var configurations = string.Join(';', TargetInfo.GetAllTargets().Select(p => VSUtility.GetConfigName(p)).Distinct());

                List<string> propertyGroups = [];
                foreach (var targetInfo in TargetInfo.GetAllTargets())
                {
                    bool isOptimize = targetInfo.Config.IsOptimized();
                    List<string> defineConstants = ["$(DefineConstants)"];
                    if (targetInfo.Editor)
                    {
                        defineConstants.Add("WITH_EDITOR");
                    }
                    var outputPath = project.Group.Output(targetInfo, FolderPolicy.PathType.Windows);
                    string propertyGroup = $"""

  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{VSUtility.GetConfigName(targetInfo)}|{targetInfo.Platform.Name}'">
    <OutputPath>{outputPath}</OutputPath>
    <Optimize>{(isOptimize ? "true" : "false")}</Optimize>
    <DefineConstants>$(DefineConstants);{(targetInfo.Editor ? "WITH_EDITOR" : string.Empty)};</DefineConstants>
    <PlatformTarget>{VSUtility.GetArchitectureName(targetInfo.Platform.Architecture)}</PlatformTarget>
  </PropertyGroup>

""";

                    propertyGroups.Add(propertyGroup);
                }

                string csprojText = $"""
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <OutputType>Library</OutputType>
    <TargetFramework>net9.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
    <AssemblyName>{project.ScriptAssemblyName}</AssemblyName>
    <RootNamespace>{namespaceName}</RootNamespace>
    <AllowUnsafeBlocks>true</AllowUnsafeBlocks>
	<AppendTargetFrameworkToOutputPath>false</AppendTargetFrameworkToOutputPath>
    <AppendRuntimeIdentifierToOutputPath>false</AppendRuntimeIdentifierToOutputPath>
    <Configurations>{configurations}</Configurations>
    <Platforms>{platforms}</Platforms>
  </PropertyGroup>
{string.Concat(propertyGroups)}{nugetPackages}{projectReferences}
  <ItemGroup>
    <Using Include="Ayla.Object">
      <Alias>Object</Alias>
    </Using>
  </ItemGroup>

</Project>

""".Replace("\r\n", "\n");

                await TextFileHelper.WriteIfChangedAsync(csprojPath, csprojText, cancellationToken);
            }

            Console.WriteLine(" Done.");
        }

        async Task BuildScriptProjects(Dictionary<ModuleProject, int> buildGraph, IEnumerable<ModuleProject> projects)
        {
            foreach (var project in projects)
            {
                BuildGraph(project, 0);
            }

            foreach (var target in buildGraph.Where(p => p.Value == 0).Select(p => p.Key))
            {
                await DotNET.BuildAsync(target.ScriptProjectFileName, buildTarget, cancellationToken);
            }

            void BuildGraph(ModuleProject target, int depth)
            {
                var rule = target.GetRule(buildTarget);
                if (rule.Scriptable.Enabled)
                {
                    if (buildGraph.TryGetValue(target, out var existingDepth))
                    {
                        if (depth > existingDepth)
                        {
                            buildGraph[target] = depth;
                        }
                    }
                    else
                    {
                        buildGraph.Add(target, depth);
                    }
                }

                var children = solution.FindDepends(target.GetResolver(buildTarget).DependencyModuleNames).OfType<ModuleProject>();
                foreach (var child in children)
                {
                    BuildGraph(child, depth + 1);
                }
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
                    if (outputs.Length == 0)
                    {
                        outputs = new string[1];
                    }
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
