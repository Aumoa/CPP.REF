// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class RHI : ModuleRules
{
    public RHI(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "ApplicationCore"
        });

        PublicDisableWarnings.Add(4324);

        if (targetInfo.Platform.Group == PlatformGroup.Windows)
        {
            PrivateAdditionalLibraries.AddRange(new[]
            {
                "dxgi.lib",
                "d3d12.lib",
                "d3d11.lib",
                "d2d1.lib",
                "User32.lib",
                "dwrite.lib",
            });
        }
    }
}