﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using BuildTool;

public class GameAppRule : ModuleRule
{
    public GameAppRule()
    {
        Category = ModuleCategory.Game;
        FilterPath = "Test";
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