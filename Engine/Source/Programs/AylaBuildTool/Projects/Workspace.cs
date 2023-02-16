// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Compilation;
using AE.ProjectFiles.VisualStudio;
using AE.Rules;
using AE.Source;

using System.Reflection;

namespace AE.Projects;

public class Workspace
{
    private readonly SourceCodeDirectory EngineDirectory;

    public string TargetName { get; private set; }

    public SourceCodeDirectory TargetDirectory => EngineDirectory;

    public List<CXXProject> CXXProjects { get; private set; } = new();

    public List<CSProject> CSProjects { get; private set; } = new();

    public Workspace(string EnginePath)
    {
        EngineDirectory = new SourceCodeDirectory(EnginePath);
        TargetName = "Engine";
    }

    public async Task GenerateDirectoriesAsync(CancellationToken CToken = default)
    {
        await EngineDirectory.GenerateDirectoriesAsync(CToken);
    }

    public async Task GenerateProjectFilesAsync(CancellationToken CToken = default)
    {
        var TargetInfo = new TargetInfo()
        {
            Name = TargetName
        };

        foreach (var TargetRule in Directory.GetFiles(EngineDirectory.SourceDirectory, "*.Target.cs", SearchOption.AllDirectories))
        {
            Type RuleClass = await CSCompiler.LoadClassAsync<TargetRules>(TargetRule, CToken);
            ConstructorInfo? Ctor = RuleClass.GetConstructor(new[] { typeof(TargetInfo) });
            if (Ctor == null)
            {
                throw new InvalidOperationException($"Cannot load TargetRules class from {TargetRule} source code. Constructor not found.");
            }

            TargetRules Rules = (TargetRules)Ctor.Invoke(new object[] { TargetInfo });
            CXXProject Project = new(TargetRule, Rules);
            await Project.CompileModulesAsync(CToken);
            CXXProjects.Add(Project);
        }

        foreach (var ProjectFile in Directory.GetFiles(EngineDirectory.ProgramsDirectory, "*.csproj", SearchOption.AllDirectories))
        {
            CSProjects.Add(new CSProject(ProjectFile));
        }
    }
}
