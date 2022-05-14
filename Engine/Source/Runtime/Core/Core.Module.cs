﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class Core : ModuleRule
{
    public Core()
    {
        TargetType = TargetType.Engine;
        RelativePath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");
        NonUnityBuild = true;

        PublicDisableWarnings.AddRange(new[]
        {
            4251,
            4275,
        });

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PrivatePreprocessorDefines.AddRange(new[]
            {
                "NOMINMAX",
                "WIN32_LEAN_AND_MEAN"
            });

            PrivateAdditionalLibraries.AddRange(new[]
            {
                "psapi.lib",
                "dbghelp.lib"
            });
            
            PrivateDisableWarnings.AddRange(new[]
            {
                5105,
                4005,
                5106,
            });
        }
    }
}
