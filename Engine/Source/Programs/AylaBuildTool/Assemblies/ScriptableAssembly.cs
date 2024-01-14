// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Reflection;

using AE.Compilation;
using AE.Exceptions;
using AE.IO;
using AE.Rules;
using AE.Source;

namespace AE.Assemblies;

public abstract class ScriptableAssembly
{
    public abstract string Name { get; }

    public abstract ProjectDirectory ProjectDirectory { get; }

    public abstract DirectoryReference SourceDirectory { get; }

    public override string ToString()
    {
        return Name;
    }

    public abstract FileReference ScriptFile { get; }

    public bool IsProgram => ProjectDirectory.Source.Programs.Includes(ScriptFile);

    protected abstract bool HasAssembly { get; }

    protected abstract string ScriptIdentify { get; }

    public string RelativeDirectory
    {
        get
        {
            if (IsProgram)
            {
                return "Programs";
            }
            else
            {
                string root = Global.EngineDirectory == ProjectDirectory ? "Engine" : "Game";
                return root + '/' + SourceDirectory.GetParent().GetRelativePath(ProjectDirectory.Source.Root);
            }
        }
    }

    protected Assembly? CachedAssembly => assembly;

    private CancellationTokenSource? cancellationTokenSource;
    private Task? compileAssemblyTask;
    private Assembly? assembly;

    public Task CompileScriptableAssemblyAsync(CancellationToken cancellationToken = default)
    {
        var scriptFile = ScriptFile;

        if (scriptFile == null || HasAssembly == false)
        {
            return Task.CompletedTask;
        }

        lock (scriptFile)
        {
            if (compileAssemblyTask == null)
            {
                cancellationTokenSource = new CancellationTokenSource();
                compileAssemblyTask = InternalCompileScriptableAssemblyAsync(scriptFile, cancellationTokenSource.Token);
            }
        }

        using var register = cancellationToken.Register(() => cancellationTokenSource!.Cancel());
        return compileAssemblyTask;
    }

    private async Task InternalCompileScriptableAssemblyAsync(FileReference scriptFile, CancellationToken cancellationToken = default)
    {
        if (scriptFile.IsExists == false)
        {
            throw new TerminateException(KnownErrorCode.TargetNotFound, CoreStrings.Errors.TargetRuleNotFound(ProjectDirectory.Source.Root, scriptFile.FileName));
        }

        var assemblyCacheDirectory = ProjectDirectory.Intermediate.Assemblies;
        var textCache = assemblyCacheDirectory.GetFile($"{Name}.{ScriptIdentify}.cs");
        var assemblyCache = assemblyCacheDirectory.GetFile($"{Name}.{ScriptIdentify}.dll");

        assemblyCacheDirectory.Create();
        string? scriptCode = await scriptFile.ReadAllTextAsync(cancellationToken);

        try
        {
            if (textCache.IsExists)
            {
                var cachedScriptcode = await textCache.ReadAllTextAsync(cancellationToken);
                if (scriptCode == cachedScriptcode)
                {
                    if (assemblyCache.IsExists)
                    {
                        assembly = Assembly.LoadFile(assemblyCache.Value);
                        cancellationToken.ThrowIfCancellationRequested();
                    }
                    else
                    {
                        Console.WriteLine("Assembly is newer. Assembly {0} is not exists.", assemblyCache);
                    }
                }
                else
                {
                    Console.WriteLine("Assembly is newer. TextCache {0} is changed.", textCache);
                }
            }
            else
            {
                Console.WriteLine("Assembly is newer. TextCache {0} doesn't exists.", textCache);
            }
        }
        catch
        {
            textCache.Delete();
            assemblyCache.Delete();
        }

        if (assembly != null)
        {
            return;
        }

        var referencedAssemblies = new string[]
        {
            typeof(object).Assembly.Location,
            Assembly.Load("System.Runtime").Location,
            Assembly.Load("System.Collections").Location,
            typeof(ModuleRules).Assembly.Location
        };

        await Task.WhenAll(
            CSCompiler.CompileToAsync(Name, assemblyCache, new string[] { scriptFile }, referencedAssemblies, cancellationToken),
            textCache.WriteAllTextAsync(scriptCode, cancellationToken)
            );

        assembly = Assembly.LoadFile(assemblyCache);
    }
}
