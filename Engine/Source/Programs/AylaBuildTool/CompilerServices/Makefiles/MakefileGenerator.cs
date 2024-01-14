// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Data;
using AE.Extensions;
using AE.IO;
using AE.Projects;

using Microsoft.CodeAnalysis;

namespace AE.CompilerServices.Makefiles;

public static class MakefileGenerator
{
    public struct SavingScope : IAsyncDisposable
    {
        public SavingScope()
        {
        }

        public async ValueTask DisposeAsync()
        {
            await MakefileGenerator.SaveMakefilesAsync();
        }
    }

    private static Makefile[]? s_Makefiles;

    public static IReadOnlyCollection<Makefile> Makefiles => s_Makefiles ?? throw new InvalidOperationException();

    private static async Task<Makefile> CreateTask(ModuleInformation moduleInfo, CancellationToken cancellationToken)
    {
        var config = Target.Config;
        var makeDir = moduleInfo.ProjectDir.Intermediate.Makefiles.GetConfigDirectory(config);
        var makefileReference = makeDir.GetFile(moduleInfo.Name + ".amake");

        var makefile = await Makefile.LoadFromFileAsync(moduleInfo, makefileReference, cancellationToken);

        var caches = makefile.Caches.ToDictionary(p => p.SourceCache.Path, p => p);
        List<MakefileCompile> shaderFiles = new();
        List<MakefileCompile> cppFiles = new();

        foreach (var sourceFile in moduleInfo.SourceFiles)
        {
            if (sourceFile.IsShaderCode())
            {
                shaderFiles.Add(GenerateCompileNode(sourceFile, caches, moduleInfo));
            }
            else if (sourceFile.IsSourceCode())
            {
                cppFiles.Add(GenerateCompileNode(sourceFile, caches, moduleInfo));
            }
        }

        if (shaderFiles.Any())
        {
            var dependsOn = Task.WhenAll(shaderFiles.Select(p => p.CompletedTask));
            foreach (var cppFile in cppFiles)
            {
                cppFile.DependsOn = dependsOn;
            }
        }

        makefile.MakefileCompiles = shaderFiles.Concat(cppFiles).ToArray();
        makefile.MakefileLink = new MakefileLink { ModuleInfo = moduleInfo, IsRequired = makefile.MakefileCompiles.Any(p => p.IsRequired) };
        return makefile;
    }

    public static void Clear()
    {
        s_Makefiles = null;
    }

    public static async Task GenerateAsync(CancellationToken cancellationToken = default)
    {
        var tasks = ModuleDependencyCache.Modules.Select(p => CreateTask(p, cancellationToken));
        s_Makefiles = await Task.WhenAll(tasks);

        var makefileDict = s_Makefiles.ToDictionary(p => p.ModuleInfo.Name, p => p);
        foreach (var makefile in s_Makefiles)
        {
            var depends = makefile.ModuleInfo.DependModules.Select(GetMakefileDepends).ToList();
            depends.AddRange(makefile.MakefileCompiles.Select(p => p.CompletedTask));
            if (depends.Any())
            {
                makefile.MakefileLink.DependsOn = Task.WhenAll(depends);
            }
        }

        Task GetMakefileDepends(string moduleName)
        {
            if (makefileDict.TryGetValue(moduleName, out var makefile))
            {
                return makefile.MakefileLink.CompletedTask;
            }
            else
            {
                return Task.CompletedTask;
            }
        }
    }

    public static Task SaveMakefilesAsync(CancellationToken cancellationToken = default)
    {
        List<Task> tasks = new();
        foreach (var makefile in Makefiles)
        {
            tasks.Add(makefile.SaveMakefileCacheAsync(cancellationToken));
        }
        return Task.WhenAll(tasks);
    }

    private static MakefileCompile GenerateCompileNode(FileReference sourceFile, Dictionary<string, MakefileSourceCache> caches, ModuleInformation moduleInfo)
    {
        if (caches.TryGetValue(sourceFile, out MakefileSourceCache? sourceCache))
        {
            if (sourceCache.IsNewer() == false)
            {
                return new MakefileCompile()
                {
                    SourceFile = sourceFile,
                    Cache = sourceCache,
                    ModuleInfo = moduleInfo,
                    IsRequired = false
                };
            }
        }

        // source is newer.
        return new()
        {
            SourceFile = sourceFile,
            Cache = null,
            ModuleInfo = moduleInfo,
            IsRequired = true
        };
    }
}
