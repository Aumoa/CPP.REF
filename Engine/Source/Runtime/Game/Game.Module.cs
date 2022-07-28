// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using BuildTool;

public class GameRule : ModuleRule
{
    public GameRule()
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

            PublicAdditionalLibraries.AddRange(new[]
            {
                "DXGI.lib",
                "D3D12.lib",
                "D3DCompiler.lib"
            });
        }
    }
}