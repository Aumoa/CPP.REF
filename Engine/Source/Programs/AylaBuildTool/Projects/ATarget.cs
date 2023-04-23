// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Compilation;
using AE.Exceptions;
using AE.Misc;
using AE.Rules;
using AE.Source;

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;

namespace AE.Projects;

public class ATarget
{
    [SetsRequiredMembers]
    public ATarget(ProjectDirectory ProjectDirectory, string SourceCode)
    {
        this.TargetName = Path.GetFileNameWithoutExtension(SourceCode.Replace(".Target.cs", ""));
        this.SourceCode = SourceCode;
        this.RuleName = TargetName;
        this.ProjectDirectory = ProjectDirectory;
    }

    public required string TargetName { get; init; }

    public required string SourceCode { get; init; }

    public required string RuleName { get; init; }

    public required ProjectDirectory ProjectDirectory { get; init; }

    private readonly AsyncLock CachedAssemblyLock = new();
    private Assembly? CachedAssembly;

    public override string ToString()
    {
        return RuleName;
    }

    public async Task ConfigureAsync(CancellationToken SToken = default)
    {
        using (var LockState = await CachedAssemblyLock.LockAsync())
        {
            if (CachedAssembly == null)
            {
                string Filename = SourceCode;
                string TargetCodePath = Path.Combine(ProjectDirectory.Source.Root, Filename);
                if (TargetCodePath == null)
                {
                    throw new TerminateException(3, CoreStrings.Errors.TargetRuleNotFound, ProjectDirectory.Source.Root, Filename);
                }

                string AssemblyCacheDirectory = ProjectDirectory.Intermediate.Makefiles;
                string TextCache = Path.Combine(AssemblyCacheDirectory, TargetName + ".Target.cs");
                string RuleAssemblyCache = Path.Combine(AssemblyCacheDirectory, TargetName + ".Target.dll");

                // Create directory if not exists.
                Directory.CreateDirectory(AssemblyCacheDirectory);

                try
                {
                    if (File.Exists(TextCache))
                    {
                        if (File.ReadAllText(TextCache) == File.ReadAllText(TargetCodePath))
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

                    await CSCompiler.CompileToAsync(TargetName, RuleAssemblyCache, new string[] { TargetCodePath }, ReferencedAssemblies, SToken);
                    await File.WriteAllTextAsync(TextCache, await File.ReadAllTextAsync(TargetCodePath), SToken);

                    CachedAssembly = Assembly.LoadFile(RuleAssemblyCache);
                    Console.WriteLine("TextCache({0}), AssemblyCache({1}) is generated.", TextCache, RuleAssemblyCache);
                }
            }
        }
    }

    public TargetRules GenerateTargetRule(TargetInfo Info)
    {
        Debug.Assert(CachedAssembly != null, "Need configure first.");

        Type? RuleType = null;
        foreach (var ContainedType in CachedAssembly.GetTypes())
        {
            if (ContainedType.IsAssignableTo(typeof(TargetRules)))
            {
                if (RuleType != null)
                {
                    throw new TerminateException(3, CoreStrings.Errors.RuleClassDefinitions, TargetName);
                }

                RuleType = ContainedType;
            }
        }

        if (RuleType == null)
        {
            throw new TerminateException(3, CoreStrings.Errors.RuleClassDefinitions, TargetName);
        }

        ConstructorInfo? Ctor = RuleType.GetConstructor(new[] { typeof(TargetInfo) });
        if (Ctor == null)
        {
            throw new TerminateException(4, CoreStrings.Errors.TargetRuleConstructorNotFound, RuleType.Name);
        }

        return (TargetRules)Ctor.Invoke(new object?[] { Info });
    }
}
