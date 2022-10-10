// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using BuildTool;

public class NetworkRule : ModuleRule
{
    public NetworkRule()
    {
        Category = ModuleCategory.Engine;
        FilterPath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core"
        });

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PrivateAdditionalMacros.AddRange(new[]
            {
                "NOMINMAX",
                "WIN32_LEAN_AND_MEAN"
            });

            PrivateAdditionalLibraries.AddRange(new[]
            {
                "ws2_32.lib"
            });
        }
    }
}