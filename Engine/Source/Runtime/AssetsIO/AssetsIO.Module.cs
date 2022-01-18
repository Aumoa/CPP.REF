// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class AssetsIO : ModuleRule
{
    public AssetsIO()
    {
        TargetType = TargetType.Engine;

        RelativePath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "PlatformGeneric",
        });
    }
}