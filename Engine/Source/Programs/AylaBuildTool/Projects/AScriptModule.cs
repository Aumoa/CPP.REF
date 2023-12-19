// Copyright 2020-2023 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

using AE.BuildSettings;
using AE.ProjectFiles.VisualStudio;
using AE.Rules;
using AE.Source;

namespace AE.Projects;

public class AScriptModule : IAModule
{
    public readonly string[] DependModules;

    [SetsRequiredMembers]
    public AScriptModule(ProjectDirectory ProjectDirectory, string CurrentDirectory, ACXXModule NativeModule)
    {
        this.ModuleName = Path.GetFileNameWithoutExtension(CurrentDirectory);
        this.SourcePath = CurrentDirectory;
        this.ProjectDirectory = ProjectDirectory;

        var SolutionPreviewTarget = new TargetInfo
        {
            BuildConfiguration = new()
            {
                Platform = TargetPlatform.Win64,
                Configuration = Configuration.Development
            }
        };

        var SolutionPreviewRule = new TargetRules(SolutionPreviewTarget);
        SolutionPreviewRule.bEditor = true;
        var ModuleRule = NativeModule.GenerateModuleRule(SolutionPreviewRule);

        DependModules = ModuleRule.PublicDependencyModuleNames.Concat(ModuleRule.PrivateDependencyModuleNames).ToArray();
    }

    public required string ModuleName { get; init; }

    public required string SourcePath { get; init; }

    public required ProjectDirectory ProjectDirectory { get; init; }

    public bool IsInProgramsDirectory => false;

    public override string ToString()
    {
        return $"Script \"{ModuleName}\"";
    }
}
