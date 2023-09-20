// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Projects;
using AE.Rules;

namespace AE.SourceTree;

public sealed record MakefileCompile
{
    public required string FilePath { get; init; }

    public required MakefileSourceCache? Cache { get; set; }

    public required ModuleInformation ModuleInfo { get; init; }

    public string GetIntermediateOutputPath(TargetRules Rule)
    {
        return ModuleInfo.ProjectDir.GenerateIntermediateOutput(Rule.Target.BuildConfiguration, ModuleInfo.Name);
    }

    public IEnumerable<string> CollectCompilerIncludePaths()
    {
        return ModuleInfo.PrivateIncludePaths.Concat(new[] { ModuleInfo.GeneratedIncludePath, ModuleInfo.GeneratedShaderPath });
    }

    public IEnumerable<(string, string?)> CollectCompilerMacros(TargetRules Rule)
    {
        List<(string, string?)> Macros = new();

        // Rule based macros.
        PlatformGroup.ForEach(p =>
        {
            bool bCurrent = p == Rule.Target.BuildConfiguration.Platform.Group;
            Macros.Add(($"PLATFORM_{p.GroupName.ToUpper()}", bCurrent ? "1" : "0"));
        });

        bool bShipping = Rule.Target.BuildConfiguration.Configuration == Configuration.Shipping;
        Macros.Add(("SHIPPING", bShipping ? "1" : "0"));

        bool bEditor = Rule.bEditor;
        Macros.Add(("WITH_EDITOR", bEditor ? "1" : "0"));

        bool bDoCheck = !bShipping;
        Macros.Add(("DO_CHECK", bDoCheck ? "1" : "0"));

        // User-defined macros.
        Macros.AddRange(ModuleInfo.PrivateAdditionalMacros.Select(p => (p, (string?)null)));

        // Library import macros.
        Macros.AddRange(ModuleInfo.DependModules.Select(p =>
        {
            string API = $"{p.ToUpper()}_API";
            return (API, (string?)"PLATFORM_SHARED_IMPORT");
        }));

        string API = $"{ModuleInfo.Name.ToUpper()}_API";
        Macros.Add((API, "PLATFORM_SHARED_EXPORT"));

        // Common macros.
        Macros.AddRange(new (string, string?)[] { ("UNICODE", null), ("_UNICODE", null) });
        return Macros;
    }
}
