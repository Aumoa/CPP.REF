﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class WindowsConsole : ModuleRule
{
    public WindowsConsole()
    {
        TargetType = TargetType.Engine;
        ModuleType = ModuleType.ConsoleApplication;
        NonUnityBuild = true;

        RelativePath = "Engine.Platform";
        TargetName = "$(SolutionName)";

        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreSObject",
            "Console",
        });

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PublicDependencyModuleNames.AddRange(new[]
            {
                "WindowsCommon"
            });
        }
        else
        {
            ModuleType = ModuleType.None;
        }
    }
}