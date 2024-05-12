// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public sealed class MakefileCompile : MakefileNode
{
    public required FileReference SourceFile { get; init; }

    public required MakefileSourceCache? Cache { get; set; }

    public required ModuleInformation ModuleInfo { get; init; }

    public required bool IsRequired { get; init; }

    public DirectoryReference BuildsOut()
    {
        return ModuleInfo.ProjectDir.Intermediate.BuildsOut(Target.Config, ModuleInfo.Name);
    }

    public IEnumerable<string> CollectCompilerIncludePaths()
    {
        return ModuleInfo.PrivateIncludePaths.Concat(new[] { ModuleInfo.GeneratedIncludePath, ModuleInfo.GeneratedShaderPath });
    }

    public IEnumerable<(string, string?)> CollectCompilerMacros()
    {
        List<(string, string?)> macros = new();

        // Rule based macros.
        PlatformGroup.ForEach(p =>
        {
            bool isCurrent = p == Target.Platform.Group;
            macros.Add(($"PLATFORM_{p.GroupName.ToUpper()}", isCurrent ? "1" : "0"));
        });

        bool isShipping = Target.Configuration == Configuration.Shipping;
        macros.Add(("SHIPPING", isShipping ? "1" : "0"));

        bool isEditor = Target.IsEditor;
        macros.Add(("WITH_EDITOR", isEditor ? "1" : "0"));

        bool doCheck = !isShipping;
        macros.Add(("DO_CHECK", doCheck ? "1" : "0"));

        macros.Add(("PLATFORM_STRING", $"TEXT(\"{Target.Platform.ToString()}\")"));
        macros.Add(("CONFIG_STRING", $"TEXT(\"{Target.Configuration.ToString()}\")"));

        // User-defined macros.
        macros.AddRange(ModuleInfo.PrivateAdditionalMacros.Select(p => (p, (string?)null)));

        // Library import macros.
        macros.AddRange(ModuleInfo.DependModules.Select(p =>
        {
            string api = $"{p.ToUpper()}_API";
            return (api, (string?)"PLATFORM_SHARED_IMPORT");
        }));

        string api = $"{ModuleInfo.Name.ToUpper()}_API";
        macros.Add((api, "PLATFORM_SHARED_EXPORT"));

        // Common macros.
        macros.AddRange(new (string, string?)[] { ("UNICODE", null), ("_UNICODE", null) });
        return macros;
    }
}
