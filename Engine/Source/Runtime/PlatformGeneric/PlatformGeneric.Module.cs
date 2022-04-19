// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

public class PlatformGeneric : ModuleRule
{
    public PlatformGeneric()
    {
        TargetType = TargetType.Engine;
        RelativePath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreSObject",
            "CoreNumerics"
        });
    }
}