// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using BuildTool;

public class AylaRule : ModuleRule
{
    public AylaRule()
    {
        Category = ModuleCategory.Game;
        FilterPath = "Games";
        ModuleType = ModuleType.Application;

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");
        
        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Game",
        });
    }
}