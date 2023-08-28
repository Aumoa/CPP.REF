// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Projects;
using AE.Rules;

namespace AE.SourceTree;

public sealed record MakefileCompile
{
    public required string Path { get; init; }

    public required MakefileSourceCache? Cache { get; set; }

    public required ModuleInformation ModuleInfo { get; init; }

    public string GetIntermediateOutputPath(TargetRules Rule)
    {
        return ModuleInfo.ProjectDir.GenerateIntermediateOutput(Rule.Target.BuildConfiguration, ModuleInfo.Name);
    }
}
