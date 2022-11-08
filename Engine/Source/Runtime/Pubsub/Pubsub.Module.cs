// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using BuildTool;

public class PubsubRule : ModuleRule
{
    public PubsubRule()
    {
        Category = ModuleCategory.Engine;
        FilterPath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Network",
            "amqpcpp"
        });
    }
}