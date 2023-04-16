// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Rules;

public class ModuleRules : SerializableRule
{
    public TargetRules TargetRule { get; }

    public ModuleRules(TargetRules TargetRule)
    {
        this.TargetRule = TargetRule;
        this.Name = GetType().Name;
    }

    public string GenerateSafeAPIName()
    {
        return Name.ToUpper() + "_API";
    }

    public required string Name { get; init; }

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
