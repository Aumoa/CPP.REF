// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Compilation;
using AE.Exceptions;
using AE.Misc;
using AE.Platform;
using AE.Rules;
using AE.Source;

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using System.Text.Json.Nodes;

namespace AE.Projects;

public class AModule
{
    [SetsRequiredMembers]
    public AModule(ProjectDirectory ProjectDirectory, string SourceRelativePath)
    {
        string SourcePath = Path.Combine(ProjectDirectory.Source.Root, SourceRelativePath);
        this.ModuleName = Path.GetFileNameWithoutExtension(SourcePath);
        this.RuleName = this.ModuleName;
        this.ProjectDirectory = ProjectDirectory;
        this.ModuleSourceDirectory = SourcePath;
    }

    public required string ModuleName { get; init; }

    public required string RuleName { get; init; }
    
    public required ProjectDirectory ProjectDirectory { get; init; }

    public required string ModuleSourceDirectory { get; init; }

    private readonly AsyncLock CachedAssemblyLock = new();
    private Assembly? CachedAssembly;

    public override string ToString()
    {
        return RuleName;
    }

    public async Task ConfigureAsync(CancellationToken SToken = default)
    {
        using (var LockState = await CachedAssemblyLock.LockAsync(SToken))
        {
            if (CachedAssembly == null)
            {
                string? ModuleCodePath = null;

                foreach (var Filename in Directory.GetFiles(ModuleSourceDirectory))
                {
                    if (Filename.EndsWith(".Module.cs"))
                    {
                        if (ModuleCodePath != null)
                        {
                            throw new TerminateException(2, CoreStrings.Errors.ModuleRuleDuplicated, ModuleSourceDirectory);
                        }

                        ModuleCodePath = Filename;
                    }
                }

                if (ModuleCodePath == null)
                {
                    throw new TerminateException(3, CoreStrings.Errors.ModuleRuleNotFound, ModuleSourceDirectory);
                }

                string AssemblyCacheDirectory = Path.Combine(ProjectDirectory.Intermediate.Makefiles, ModuleName);
                string TextCache = Path.Combine(AssemblyCacheDirectory, "Module.cs");
                string RuleAssemblyCache = Path.Combine(AssemblyCacheDirectory, "Module.dll");

                // Create directory if not exists.
                Directory.CreateDirectory(AssemblyCacheDirectory);

                try
                {
                    if (File.Exists(TextCache))
                    {
                        if (File.ReadAllText(TextCache) == File.ReadAllText(ModuleCodePath))
                        {
                            if (File.Exists(RuleAssemblyCache))
                            {
                                CachedAssembly = Assembly.LoadFile(RuleAssemblyCache);
                            }
                            else
                            {
                                Console.WriteLine("Makefile is newer. Assembly {0} is not exists.", RuleAssemblyCache);
                            }
                        }
                        else
                        {
                            Console.WriteLine("Makefile is newer. TextCache {0} is changed.", TextCache);
                        }
                    }
                    else
                    {
                        Console.WriteLine("Makefile is newer. TextCache {0} doesn't exists.", TextCache);
                    }
                }
                catch
                {
                    if (File.Exists(TextCache))
                    {
                        File.Delete(TextCache);
                    }
                    if (File.Exists(RuleAssemblyCache))
                    {
                        File.Delete(RuleAssemblyCache);
                    }
                }

                if (CachedAssembly == null)
                {
                    var ReferencedAssemblies = new string[]
                    {
                        typeof(object).Assembly.Location,
                        Assembly.Load("System.Runtime").Location,
                        Assembly.Load("System.Collections").Location,
                        typeof(ModuleRules).Assembly.Location
                    };

                    await CSCompiler.CompileToAsync(ModuleName, RuleAssemblyCache, new string[] { ModuleCodePath }, ReferencedAssemblies, SToken);
                    await File.WriteAllTextAsync(TextCache, await File.ReadAllTextAsync(ModuleCodePath), SToken);

                    CachedAssembly = Assembly.LoadFile(RuleAssemblyCache);
                    Console.WriteLine("TextCache({0}), AssemblyCache({1}) is generated.", TextCache, RuleAssemblyCache);
                }
            }
        }
    }

    private Dictionary<TargetRules, ModuleRules> RulesCache = new();

    public ModuleRules GenerateModuleRule(TargetRules Rule)
    {
        Debug.Assert(CachedAssembly != null, "Need configure first.");

        lock (RulesCache)
        {
            if (RulesCache.TryGetValue(Rule, out var ModuleRule) == false)
            {
                Type? RuleType = CachedAssembly.GetType(ModuleName);
                if (RuleType == null)
                {
                    throw new TerminateException(3, CoreStrings.Errors.RuleClassDefinitions, ModuleName);
                }

                ConstructorInfo? Ctor = RuleType.GetConstructor(new[] { typeof(TargetRules) });
                if (Ctor == null)
                {
                    throw new TerminateException(4, CoreStrings.Errors.ModuleRuleConstructorNotFound, RuleType.Name);
                }

                ModuleRule = (ModuleRules)Ctor.Invoke(new object?[] { Rule });
            }

            return ModuleRule;
        }
    }

    public Task ScanDependenciesAsync(TargetRules Rule, ModuleCompiler Compiler, CompileTree Tree, ModuleDependenciesResolver Resolver, CancellationToken SToken = default)
    {
        ModuleRules ModuleRule = GenerateModuleRule(Rule);
        string Includes = string.Join(" ", Resolver.ResolveIncludeDirectories(ModuleRule, true).Select(p => $"/I\"{p}\""));
        string Defines = string.Join(" ", Resolver.ResolveDefines(ModuleRule, true).Select(p => $"/D \"{p}\""));

        ProjectDirectory ProjectDir = Resolver.GetModuleDirectory(this);

        string Configuration = Rule.Target.BuildConfiguration.Configuration.ToString();
        string OutBase = Path.Combine(ProjectDir.Intermediate.Build, Configuration, ModuleName);
        Directory.CreateDirectory(OutBase);

        string CompilerOptions = $"/nologo /scanDependencies- /std:c++20 /c /EHsc {Includes} {Defines}";

        return ExecuteCompilerAsync(Rule, Compiler, async (SourceCode) =>
        {
            string Filename = Path.Combine(OutBase, Path.GetFileNameWithoutExtension(SourceCode));
            string OutFilename = Path.ChangeExtension(Filename, ".obj");
            string CacheFilename = Path.ChangeExtension(Filename, ".abin");

            JsonNode? TreeNode = JsonNode.Parse(await Compiler.RunCompilerAsync(SourceCode, $"{SourceCode} {CompilerOptions} /Fo\"{Filename}\"", SToken));
            var CompileItem = new ModuleCompileItem(this, new() { Defines = Defines, Includes = Includes }, SourceCode, Tree, TreeNode);
            Tree.AddCompileItem(CompileItem);
        }, Resolver, SToken);
    }

    public Task CompileAsync(TargetRules Rule, ModuleCompiler Compiler, CompileTree Tree, ModuleDependenciesResolver Resolver, CancellationToken SToken = default)
    {
        ModuleRules ModuleRule = GenerateModuleRule(Rule);
        string Includes = string.Join(" ", Resolver.ResolveIncludeDirectories(ModuleRule, true).Select(p => $"/I\"{p}\""));
        string Defines = string.Join(" ", Resolver.ResolveDefines(ModuleRule, true).Select(p => $"/D \"{p}\""));

        ProjectDirectory ProjectDir = Resolver.GetModuleDirectory(this);

        string Configuration = Rule.Target.BuildConfiguration.Configuration.ToString();
        string OutBase = Path.Combine(ProjectDir.Intermediate.Build, Configuration, ModuleName);
        Directory.CreateDirectory(OutBase);

        string CompilerOptions = $"/nologo /std:c++20 /c /EHsc {Includes} {Defines}";

        return ExecuteCompilerAsync(Rule, Compiler, async (SourceCode) =>
        {
            string Filename = Path.Combine(OutBase, Path.GetFileNameWithoutExtension(SourceCode));
            string OutFilename = Path.ChangeExtension(Filename, ".obj");
            string CacheFilename = Path.ChangeExtension(Filename, ".abin");

            string Output = await Compiler.RunCompilerAsync(SourceCode, $"{SourceCode} {CompilerOptions} /Fo\"{Filename}\"", SToken);
            Console.Write(Output);
        }, Resolver, SToken);
    }

    private async Task ExecuteCompilerAsync(TargetRules Rule, ModuleCompiler Compiler, Func<string, Task> CompileAction, ModuleDependenciesResolver Resolver, CancellationToken SToken)
    {
        List<Task> Tasks = new();

        ModuleRules ModuleRule = GenerateModuleRule(Rule);
        string Includes = string.Join(" ", Resolver.ResolveIncludeDirectories(ModuleRule, true).Select(p => $"/I\"{p}\""));
        string Defines = string.Join(" ", Resolver.ResolveDefines(ModuleRule, true).Select(p => $"/D \"{p}\""));

        ProjectDirectory ProjectDir = Resolver.GetModuleDirectory(this);

        string Configuration = Rule.Target.BuildConfiguration.Configuration.ToString();
        string OutBase = Path.Combine(ProjectDir.Intermediate.Build, Configuration, ModuleName);
        Directory.CreateDirectory(OutBase);

        string CompilerOptions = $"/nologo /scanDependencies- /std:c++20 /c /EHsc {Includes} {Defines}";

        foreach (var SourceCode in Directory.GetFiles(ModuleSourceDirectory, "*.ixx", SearchOption.AllDirectories))
        {
            Tasks.Add(CompileAction(SourceCode));
        }

        await Task.WhenAll(Tasks);
    }
}
