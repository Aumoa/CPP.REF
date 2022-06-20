// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using BuildTool;

public class ConsoleExRule : ModuleRule
{
    public ConsoleExRule()
    {
        Category = ModuleCategory.Engine;
        FilterPath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
        });
    }
}