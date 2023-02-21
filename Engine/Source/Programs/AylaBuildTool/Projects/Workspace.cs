// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Compilation;
using AE.Rules;
using AE.Source;
using AE.System;

using System.Reflection;

namespace AE.Projects;

public class Workspace
{
    public string TargetName { get; private set; }

    public SourceCodeDirectory TargetDirectory { get; }

    public SourceCodeDirectory EngineDirectory { get; }

    public List<CXXProject> CXXProjects { get; private set; } = new();

    public List<CSProject> CSProjects { get; private set; } = new();

    public string ProjectPath { get; private set; } = null!;

    public string BuildTool { get; }

    public Workspace(string EnginePath, string BuildToolPath)
    {
        EngineDirectory = new SourceCodeDirectory(EnginePath);
        TargetDirectory = EngineDirectory;
        TargetName = "Engine";
        ProjectPath = Path.Combine(EnginePath, "../");
        BuildTool = BuildToolPath;
    }

    public async Task GenerateDirectoriesAsync(CancellationToken CToken = default)
    {
        await EngineDirectory.GenerateDirectoriesAsync(CToken);
    }

    public async Task GenerateProjectFilesAsync(CancellationToken CToken = default)
    {
        var TargetInfo = new TargetInfo()
        {
            Name = TargetName,
            Configuration = BuildHostPlatform.Current.Configuration
        };

        Dictionary<string, CXXProject.ResolvedModule> Projects = new();

        foreach (var TargetRule in Directory.GetFiles(EngineDirectory.SourceDirectory, "*.Target.cs", SearchOption.AllDirectories))
        {
            Type RuleClass = await CSCompiler.LoadClassAsync<TargetRules>(TargetRule, CToken);
            ConstructorInfo? Ctor = RuleClass.GetConstructor(new[] { typeof(TargetInfo) });
            if (Ctor == null)
            {
                throw new InvalidOperationException(string.Format(CoreStrings.Errors.TargetRuleConstructorNotFound, RuleClass.Name));
            }

            TargetRules Rules = (TargetRules)Ctor.Invoke(new object[] { TargetInfo });
            CXXProject Project = new(TargetRule, Rules, EngineDirectory);
            await Project.CompileModulesAsync(Projects, CToken);
            CXXProjects.Add(Project);
        }

        foreach (var ProjectFile in Directory.GetFiles(EngineDirectory.ProgramsDirectory, "*.csproj", SearchOption.AllDirectories))
        {
            CSProjects.Add(new CSProject(ProjectFile));
        }

        foreach (var Project in CXXProjects)
        {
            Project.ResolveDependency(Projects);
        }
    }
}
