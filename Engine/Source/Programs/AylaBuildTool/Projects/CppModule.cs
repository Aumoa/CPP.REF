// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using System.Text.Json.Nodes;

using AE.Compilation;
using AE.Exceptions;
using AE.Misc;
using AE.Rules;
using AE.Source;
using AE.System;

namespace AE.Projects;

public class CppModule : Module
{
    [SetsRequiredMembers]
    public CppModule(ProjectDirectory projectDirectory, string sourceRelativePath)
    {
        string SourcePath = Path.Combine(projectDirectory.Source.Root, sourceRelativePath);
        this.Name = Path.GetFileNameWithoutExtension(SourcePath);
        this.RuleName = this.Name;
        this.ProjectDirectory = projectDirectory;
        this.SourcePath = SourcePath;
    }

    public override string Name { get; }

    public required string RuleName { get; init; }
    
    public override ProjectDirectory ProjectDirectory { get; }

    public override string SourcePath { get; }

    private readonly AsyncLock CachedAssemblyLock = new();
    private Assembly? CachedAssembly;

    public override string ToString()
    {
        return RuleName;
    }

    public bool IsInProgramsDirectory => SourcePath.StartsWith(ProjectDirectory.Source.Programs);

    public async Task ConfigureAsync(CancellationToken SToken = default)
    {
        using (var LockState = await CachedAssemblyLock.LockAsync(SToken))
        {
            if (CachedAssembly == null)
            {
                string? ModuleCodePath = null;

                foreach (var Filename in Directory.GetFiles(SourcePath))
                {
                    if (Filename.EndsWith(".Module.cs"))
                    {
                        if (ModuleCodePath != null)
                        {
                            throw new TerminateException(KnownErrorCode.ModuleDuplicate, CoreStrings.Errors.ModuleRuleDuplicated(SourcePath));
                        }

                        ModuleCodePath = Filename;
                    }
                }

                if (ModuleCodePath == null)
                {
                    throw new TerminateException(KnownErrorCode.ModuleNotFound, CoreStrings.Errors.ModuleRuleNotFound(SourcePath));
                }

                string AssemblyCacheDirectory = Path.Combine(ProjectDirectory.Intermediate.Makefiles, Name);
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

                    await CSCompiler.CompileToAsync(Name, RuleAssemblyCache, new string[] { ModuleCodePath }, ReferencedAssemblies, SToken);
                    await File.WriteAllTextAsync(TextCache, await File.ReadAllTextAsync(ModuleCodePath), SToken);

                    CachedAssembly = Assembly.LoadFile(RuleAssemblyCache);
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
                Type? RuleType = CachedAssembly.GetType(Name);
                if (RuleType == null)
                {
                    throw new TerminateException(KnownErrorCode.ConstructorNotFound, CoreStrings.Errors.RuleClassDefinitions(Name));
                }

                ConstructorInfo? Ctor = RuleType.GetConstructor(new[] { typeof(TargetRules) });
                if (Ctor == null)
                {
                    throw new TerminateException(KnownErrorCode.ConstructorNotFound, CoreStrings.Errors.ModuleRuleConstructorNotFound(RuleType.Name));
                }

                ModuleRule = (ModuleRules)Ctor.Invoke(new object?[] { Rule });
            }

            return ModuleRule;
        }
    }
}
