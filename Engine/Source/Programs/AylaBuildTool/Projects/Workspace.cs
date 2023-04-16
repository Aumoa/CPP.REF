// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Compilation;
using AE.Rules;
using AE.Source;

using System.Collections.Concurrent;
using System.Reflection;

namespace AE.Projects;

public class Workspace
{
    public string TargetName { get; private set; }

    public ProjectDirectory TargetDirectory { get; }

    public ProjectDirectory EngineDirectory { get; }

    public List<CXXProject> CXXProjects { get; private set; } = new();

    public List<CSProject> CSProjects { get; private set; } = new();

    public string ProjectPath { get; private set; } = null!;

    public string BuildTool { get; }

    public Workspace(string EnginePath, string BuildToolPath)
    {
        EngineDirectory = new ProjectDirectory() { Root = EnginePath };
        TargetDirectory = EngineDirectory;
        TargetName = "Engine";
        ProjectPath = Path.Combine(EnginePath, "../");
        BuildTool = BuildToolPath;
    }

    public async Task GenerateDirectoriesAsync(CancellationToken CToken = default)
    {
        await Task.Yield();
        throw new Exception();
        //await EngineDirectory.GenerateDirectoriesRecursiveAsync(CToken);
    }

    public async Task GenerateProjectFilesAsync(TargetInfo InTargetInfo, CancellationToken CToken = default)
    {
        ConcurrentDictionary<string, CXXProject.ResolvedModule> Projects = new();

        TargetRules? Rules = null;
        //foreach (var TargetRule in Directory.GetFiles(EngineDirectory.Source, "*.Target.cs", SearchOption.AllDirectories))
        //{
        //    string Filename = Path.GetFileNameWithoutExtension(TargetRule);
        //    Filename = Filename.Replace(".Target", "");
        //    if (InTargetInfo.Name != null && Filename == InTargetInfo.Name)
        //    {
        //        Type RuleClass = await CSCompiler.LoadClassAsync<TargetRules>(TargetRule, CToken);
        //        ConstructorInfo? Ctor = RuleClass.GetConstructor(new[] { typeof(TargetInfo) });
        //        if (Ctor == null)
        //        {
        //            throw new InvalidOperationException(string.Format(CoreStrings.Errors.TargetRuleConstructorNotFound, RuleClass.Name));
        //        }

        //        Rules = (TargetRules)Ctor.Invoke(new object[] { InTargetInfo });
        //        break;
        //    }
        //}

        //if (InTargetInfo.Name != null && Rules == null)
        //{
        //    throw new InvalidOperationException(string.Format(CoreStrings.Errors.TargetNotFoundException, InTargetInfo.Name));
        //}

        if (Rules == null)
        {
            // Search all modules.
            //foreach (var TargetRule in Directory.GetFiles(EngineDirectory.Source, "*.Target.cs", SearchOption.AllDirectories))
            //{
            //    CXXProject Project;

            //    Type RuleClass = await CSCompiler.LoadClassAsync<TargetRules>(TargetRule, CToken);
            //    ConstructorInfo? Ctor = RuleClass.GetConstructor(new[] { typeof(TargetInfo) });
            //    if (Ctor == null)
            //    {
            //        throw new InvalidOperationException(string.Format(CoreStrings.Errors.TargetRuleConstructorNotFound, RuleClass.Name));
            //    }

            //    string ProjectName = Directory.GetParent(TargetRule)!.Name;

            //    TargetRules CurrentRules = (TargetRules)Ctor.Invoke(new object[] { InTargetInfo });
            //    CurrentRules.bBuildAllModules = true;
            //    Project = new(TargetRule, CurrentRules, EngineDirectory);

            //    await Project.CompileModulesAsync(Projects, CToken);
            //    CXXProjects.Add(Project);
            //}
        }
        else
        {
            Dictionary<string, (string Filename, CXXProject Project)> ModuleProjects = new();

            // Search all modules.
            //string[] TargetRuleList = Directory.GetFiles(EngineDirectory.Source, "*.Target.cs", SearchOption.AllDirectories);
            //foreach (var TargetRule in TargetRuleList)
            //{
            //    CXXProject Project = new(TargetRule, Rules, EngineDirectory);
            //    CXXProjects.Add(Project);

            //    foreach (var Module in Project.GetModules())
            //    {
            //        string ModuleName = Path.GetFileNameWithoutExtension(Module);
            //        ModuleName = ModuleName.Replace(".Module", "");
            //        ModuleProjects.Add(ModuleName, (Module, Project));
            //    }
            //}

            List<string> LoadModules = new(ModuleProjects.Count);
            List<string> LoadModulesSwap = new();
            HashSet<string> LoadedModules = new();

            Dictionary<CXXProject, HashSet<string>> CompileTargets = new(ModuleProjects.Count);
            //LoadModules.Add(Rules.TargetModuleName);

            // Search dependencies modules for current target.
            while (LoadModules.Count != 0)
            {
                // swap
                (LoadModulesSwap, LoadModules) = (LoadModules.Distinct().ToList(), LoadModulesSwap);
                LoadModules.Clear();

                foreach (var ModuleName in LoadModulesSwap)
                {
                    var (Filename, Project) = ModuleProjects[ModuleName];
                    if (CompileTargets.TryGetValue(Project, out HashSet<string>? Modules) == false)
                    {
                        Modules = new();
                        CompileTargets.Add(Project, Modules);
                    }

                    if (LoadedModules.Add(ModuleName) == false)
                    {
                        continue;
                    }

                    if (Modules.Add(Filename) == false)
                    {
                        continue;
                    }

                    var ModuleRule = await Project.CompileModuleAsync(Projects, Filename, CToken);

                    foreach (var DependModule in ModuleRule.PublicDependencyModuleNames)
                    {
                        LoadModules.Add(DependModule);
                    }
                    foreach (var DependModule in ModuleRule.PrivateDependencyModuleNames)
                    {
                        LoadModules.Add(DependModule);
                    }
                }
            }
        }

        //foreach (var ProjectFile in Directory.GetFiles(EngineDirectory.ProgramsDirectory, "*.csproj", SearchOption.AllDirectories))
        //{
        //    CSProjects.Add(new CSProject(ProjectFile));
        //}

        foreach (var Project in CXXProjects)
        {
            Project.ResolveDependency(Projects);
        }
    }
}
