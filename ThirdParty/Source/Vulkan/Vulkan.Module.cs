// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using CodeProjectConfiguration;

public class Vulkan : ModuleRule
{
    public Vulkan()
    {
        //TargetType = TargetType.ThirdParty;
        TargetType = TargetType.Engine;
        ModuleType = ModuleType.GameModule;
        RelativePath = "ThirdParty";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicIncludePaths.AddRange(new[]
        {
            // Add vulkan include path at your environment.
            "@%VULKAN_SDK%/Include"
        });

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "PlatformGeneric"
        });

        PrivateAdditionalLibraries.AddRange(new[]
        {
            "%VULKAN_SDK%/Lib/vulkan-1.lib",
        });

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PublicPreprocessorDefines.AddRange(new[]
            {
                "NOMINMAX",
                "VK_USE_PLATFORM_WIN32_KHR",
            });
        }
    }
}