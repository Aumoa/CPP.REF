// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Rules;

public class ModuleRules
{
    public enum ModuleType
    {
        Library,
        ConsoleApplication,
        Application
    }

    public TargetInfo TargetInfo { get; }

    public ModuleRules(TargetInfo TargetInfo)
    {
        this.TargetInfo = TargetInfo;
        this.Name = GetType().Name;
    }

    public string GenerateSafeAPIName()
    {
        return Name.ToUpper() + "_API";
    }

    public required string Name { get; init; }

    public ModuleType Type { get; protected set; } = ModuleType.Library;

    public List<string> PublicDependencyModuleNames { get; protected set; } = new();

    public List<string> PrivateDependencyModuleNames { get; protected set; } = new();

    public List<string> PublicIncludePaths { get; protected set; } = new();

    public List<string> PrivateIncludePaths { get; protected set; } = new();

    public List<string> PublicAdditionalMacros { get; protected set; } = new();

    public List<string> PrivateAdditionalMacros { get; protected set; } = new();

    public List<string> PublicAdditionalLibraries { get; protected set; } = new();

    public List<string> PrivateAdditionalLibraries { get; protected set; } = new();

    public List<int> PublicDisableWarnings { get; protected set; } = new();

    public List<int> PrivateDisableWarnings { get; protected set; } = new();
}
