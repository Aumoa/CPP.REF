// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class RHI : ModuleRules
{
    public RHI()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "ApplicationCore");
        AddPublicDisableWarnings(4324);

        if (TargetInfo.Platform.Group == PlatformGroup.Windows)
        {
            AddPrivateAdditionalLibraries("dxgi.lib", "d3d12.lib", "d3d11.lib", "d2d1.lib", "User32.lib", "dwrite.lib");
        }
    }
}