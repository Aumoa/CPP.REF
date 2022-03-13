// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class CoreNumerics : ModuleRule
{
    public CoreNumerics()
    {
        TargetType = TargetType.Engine;
        RelativePath = "Engine.Runtime";
        NonUnityBuild = true;

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core"
        });
    }
}