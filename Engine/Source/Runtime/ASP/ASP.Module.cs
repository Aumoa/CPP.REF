// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class ASP : ModuleRule
{
    public ASP()
    {
        TargetType = TargetType.Engine;
        NonUnityBuild = true;
        RelativePath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreSObject",
            "Sockets"
        });
    }
}