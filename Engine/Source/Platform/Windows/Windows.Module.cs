// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class Windows : ModuleRule
{
    public Windows()
    {
        TargetType = TargetType.Engine;
        ModuleType = ModuleType.Application;
        NonUnityBuild = true;

        RelativePath = "Engine.Platform";
        TargetName = "$(SolutionName)";
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "PlatformGeneric",
            "Game",
            "Console"
        });

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PublicDependencyModuleNames.AddRange(new[]
            {
                "WindowsCommon",
                "DirectX",
                //"Vulkan",
            });

            PrivateAdditionalLibraries.AddRange(new[]
            {
                "User32.lib",
                "WindowsCodecs.lib"
            });
        }
        else
        {
            ModuleType = ModuleType.None;
        }
    }
}