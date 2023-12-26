// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

using AE.Exceptions;
using AE.Extensions;
using AE.Rules;
using AE.SourceTree;
using AE.System;

using Microsoft.CodeAnalysis;

namespace AE.Projects;

public class ModuleDependenciesResolver
{
    public required TargetRules Rule { get; init; }

    public required Dictionary<string, SearchedModule> Modules { get; init; } = new();

    public required ToolChainInstallation ToolChain { get; init; }

    [SetsRequiredMembers]
    public ModuleDependenciesResolver(TargetRules Rule, Dictionary<string, SearchedModule> Modules, ToolChainInstallation ToolChain)
    {
        this.Rule = Rule;
        this.Modules = Modules;
        this.ToolChain = ToolChain;
    }

    private readonly Dictionary<string, ModuleInformation> DependencyCaches = new();

    public void Resolve()
    {
        Debug.Assert(DependencyCaches.Count == 0);
        SearchDependencyRecursive(Rule.TargetModuleName, true);
    }

    private void SearchDependencyRecursive(string Current, bool bPrimary)
    {
        if (DependencyCaches.TryGetValue(Current, out ModuleInformation? DependencyCache) == false)
        {
            var CurrentSearchedModule = Modules[Current];
            string SourcePath = CurrentSearchedModule.Location;
            var ModuleRule = CurrentSearchedModule.Rule;
            var ProjectDir = CurrentSearchedModule.ProjectDir;
            string IncludePath = Path.Combine(ProjectDir.Intermediate.Includes, ModuleRule.Name);
            string CSharpPath = Path.Combine(ProjectDir.Intermediate.CSharp, ModuleRule.Name);

            string AsFullPath(string CurrentPath)
            {
                return Path.Combine(SourcePath, CurrentPath);
            }

            List<string> IncludePaths = new();
            List<string> AdditionalMacros = new();
            List<string> ApiDescriptions = new();
            List<string> AdditionalLibs = new();
            List<int> DisableWarnings = new();

            foreach (var Depend in ModuleRule.PublicDependencyModuleNames.Concat(ModuleRule.PrivateDependencyModuleNames))
            {
                SearchDependencyRecursive(Depend, false);
                ModuleInformation? DependCache = DependencyCaches[Depend];
                IncludePaths.AddRange(DependCache.PublicIncludePaths);
                AdditionalMacros.AddRange(DependCache.PublicAdditionalMacros);
                ApiDescriptions.AddRange(DependCache.DependModules);
                ApiDescriptions.Add(Depend);
                AdditionalLibs.AddRange(DependCache.PublicAdditionalLibraries);
                DisableWarnings.AddRange(DependCache.PublicDisableWarnings);
            }

            string[] PublicIncludePaths = IncludePaths.Concat(ModuleRule.PublicIncludePaths.Select(AsFullPath)).Append(IncludePath).Distinct().ToArray();
            string[] PrivateIncludePaths = PublicIncludePaths.Concat(ModuleRule.PrivateIncludePaths.Select(AsFullPath)).Distinct().ToArray();
            string[] PublicAdditionalMacros = AdditionalMacros.Concat(ModuleRule.PublicAdditionalMacros).Distinct().ToArray();
            string[] PrivateAdditionalMacros = PublicAdditionalMacros.Concat(ModuleRule.PrivateAdditionalMacros).Distinct().ToArray();
            string[] PublicAdditionalLibs = AdditionalLibs.Concat(ModuleRule.PublicAdditionalLibraries).Distinct().ToArray();
            string[] PrivateAdditionalLibs = PublicAdditionalLibs.Concat(ModuleRule.PrivateAdditionalLibraries).Distinct().ToArray();
            int[] PublicDisableWarnings = DisableWarnings.Concat(ModuleRule.PublicDisableWarnings).Distinct().ToArray();
            int[] PrivateDisableWarnings = PublicDisableWarnings.Concat(ModuleRule.PrivateDisableWarnings).Distinct().ToArray();

            IEnumerable<string> SourceFiles = Directory.GetFiles(SourcePath, "*", SearchOption.AllDirectories);
            SourceFiles = SourceFiles.Where(SourceCodeExtensions.IsSourceCode);

            DependencyCaches[Current] = new ModuleInformation()
            {
                Name = Current,
                TargetType = bPrimary ? Rule.Type : TargetType.Module,
                ProjectDir = ProjectDir,
                SourceFiles = SourceFiles.ToArray(),
                SourcePath = SourcePath,
                GeneratedIncludePath = IncludePath,
                CSharpPath = CSharpPath,
                GeneratedShaderPath = ProjectDir.Intermediate.Shaders,
                DependModules = ApiDescriptions.Distinct().ToArray(),
                PublicIncludePaths = PublicIncludePaths,
                PrivateIncludePaths = PrivateIncludePaths,
                PublicAdditionalMacros = PublicAdditionalMacros,
                PrivateAdditionalMacros = PrivateAdditionalMacros, 
                PublicAdditionalLibraries = PublicAdditionalLibs,
                PrivateAdditionalLibraries = PrivateAdditionalLibs,
                PublicDisableWarnings = PublicDisableWarnings,
                PrivateDisableWarnings = PrivateDisableWarnings,
            };
        }
    }

    public ModuleInformation GetDependencyCache(string Module)
    {
        return DependencyCaches[Module];
    }

    public IEnumerable<ModuleRules> GetModules() => Modules.Values.Select(p => p.Rule);

    public bool HasDependModule(string InDependModuleName)
    {
        foreach (var (Name, Cache) in DependencyCaches)
        {
            if (Cache.DependModules.Contains(InDependModuleName))
            {
                return true;
            }
        }

        return false;
    }

    public async Task<int> GenerateReflectionCodesAsync(CancellationToken InCancellationToken = default)
    {
        List<ModuleInformation> HeaderToolPaths = new();
        foreach (var (Name, Cache) in DependencyCaches)
        {
            if (Cache.DependModules.Contains("CoreAObject"))
            {
                HeaderToolPaths.Add(Cache);
            }
        }

        if (HeaderToolPaths.Any())
        {
            var PSI = new ProcessStartInfo();

            if (Environment.OSVersion.Platform == PlatformID.Unix)
            {
                string BaseDir = Path.Combine(Global.EngineDirectory.Binaries.Linux, "Shipping");
                PSI.Environment["LD_LIBRARY_PATH"] = BaseDir;
                PSI.FileName = Path.Combine(BaseDir, "AylaHeaderTool");
            }
            else if (Environment.OSVersion.Platform == PlatformID.Win32NT)
            {
                PSI.FileName = Global.EngineDirectory.Binaries.Win64;
                PSI.FileName = Path.Combine(PSI.FileName, "Shipping", "AylaHeaderTool.exe");
            }
            else
            {
                throw new TerminateException(KnownErrorCode.NotSupportedBuildHostPlatform, CoreStrings.Errors.NotSupportedBuildHostPlatform);
            }

            PSI.WorkingDirectory = Global.EngineDirectory.Root;

            foreach (var Cache in HeaderToolPaths)
            {
                PSI.Arguments = $"""
                -Source "{Cache.SourcePath}" -Includes "{Cache.GeneratedIncludePath}" -PackageName "{Cache.Name}" -CSharp "{Cache.CSharpPath}.Interop"
                """;

                if (Directory.Exists(Cache.GeneratedIncludePath) == false)
                {
                    Directory.CreateDirectory(Cache.GeneratedIncludePath);
                }

                Console.WriteLine("  Running AylaHeaderTool \"{0}\"", Cache.SourcePath);
                Process? P = Process.Start(PSI);
                Debug.Assert(P != null);
                await P.WaitForExitAsync(InCancellationToken);
                if (P.ExitCode != 0)
                {
                    return P.ExitCode;
                }

                Cache.SourceFiles = Cache.SourceFiles.Concat(Directory.GetFiles(Cache.GeneratedIncludePath, "*.cpp", SearchOption.TopDirectoryOnly)).ToArray();
            }
        }

        return 0;
    }

    public async Task<Dictionary<ModuleInformation, Makefile>> GenerateMakefilesAsync(ToolChainInstallation toolChain, TargetRules Rule, CancellationToken SToken = default)
    {
        var Config = Rule.Target.BuildConfiguration;
        List<Task<(ModuleInformation, Makefile)>> Tasks = new();

        foreach (var (Name, Cache) in DependencyCaches)
        {
            string MakefileDir = Path.Combine(Cache.ProjectDir.Intermediate.Build, Config.Platform.TargetName, Config.Configuration.ToString(), Name);
            Tasks.Add(Makefile.LoadMakefileCacheAsync(toolChain, MakefileDir).ContinueWith(p =>
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
        return Items.ToDictionary(p => p.Item1, p => p.Item2);
    }
}
