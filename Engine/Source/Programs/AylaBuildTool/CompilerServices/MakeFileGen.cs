// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Extensions;
using AE.Projects;
using AE.SourceTree;
using AE.System;

using System.Data;

namespace AE.CompilerServices;

public static class MakeFileGen
{
    private static Dictionary<ModuleInformation, Makefile> s_Makefiles = new();

    private static void Clear()
    {
        lock (s_Makefiles)
        {
            s_Makefiles.Clear();
        }
    }

    public static IEnumerable<Makefile> Makefiles => s_Makefiles.Values;

    public static IEnumerable<ModuleInformation> Modules => s_Makefiles.Keys;

    public static async Task<Dictionary<ModuleInformation, Makefile>> GenerateMakefilesAsync(CancellationToken cancellationToken = default)
    {
        var config = Target.Rules.Target.BuildConfiguration;
        List<Task<(ModuleInformation, Makefile)>> Tasks = new();

        foreach (var Cache in ModuleDependencyCache.Modules)
        {
            string MakefileDir = Path.Combine(Cache.ProjectDir.Intermediate.Build, config.Platform.TargetName, config.Configuration.ToString(), Cache.Name);
            Tasks.Add(Makefile.LoadMakefileCacheAsync(MakefileDir, cancellationToken).ContinueWith(p =>
            {
                Makefile Makefile = p.Result;
                var CacheDict = Makefile.Caches.ToDictionary(p => p.SourceCache.Path, p => p);
                List<MakefileCompile> CompileItems = new();

                foreach (var SourceFile in Cache.SourceFiles)
                {
                    if (SourceCodeExtensions.IsSourceCode(SourceFile) == false)
                    {
                        continue;
                    }

                    MakefileCompile? Compile = null;

                    if (CacheDict.TryGetValue(SourceFile, out MakefileSourceCache? SourceCache))
                    {
                        if (SourceCache.IsNewer(SourceFile) == false)
                        {
                            Compile = new()
                            {
                                FilePath = SourceFile,
                                Cache = SourceCache,
                                ModuleInfo = Cache
                            };
                        }
                    }

                    // source is newer.
                    if (Compile == null)
                    {
                        Compile = new()
                        {
                            FilePath = SourceFile,
                            Cache = null,
                            ModuleInfo = Cache
                        };
                    }

                    CompileItems.Add(Compile);
                }

                Makefile.CompileItems = CompileItems.ToArray();
                Makefile.ModuleInfo = Cache;
                return (Cache, Makefile);
            }));
        }

        var Items = await Task.WhenAll(Tasks);

        lock (s_Makefiles)
        {
            foreach (var (module, makefile) in Items)
            {
                s_Makefiles.TryAdd(module, makefile);
            }
        }

        return s_Makefiles;
    }
}
