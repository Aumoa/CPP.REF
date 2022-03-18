﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class WindowsCommon : ModuleRule
{
    public WindowsCommon()
    {
        TargetType = TargetType.Engine;
        NonUnityBuild = true;
        RelativePath = "Engine.Platform";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.Add("Core");

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PrivateAdditionalLibraries.AddRange(new[]
            {
                "psapi.lib",
                "dbghelp.lib"
            });
        }
        else
        {
            ModuleType = ModuleType.None;
        }
    }
}