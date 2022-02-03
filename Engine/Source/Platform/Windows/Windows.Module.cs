// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class Windows : ModuleRule
{
    public Windows()
    {
        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            TargetType = TargetType.Engine;
            ModuleType = ModuleType.Application;

            RelativePath = "Engine.Platform";
            TargetName = "$(SolutionName)";

            PrivateIncludePaths.Add("Private");

            PublicDependencyModuleNames.AddRange(new[]
            {
                "Core",
                "PlatformGeneric",
                "Game",
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
            TargetType = TargetType.None;
            ModuleType = ModuleType.None;
        }
    }
}