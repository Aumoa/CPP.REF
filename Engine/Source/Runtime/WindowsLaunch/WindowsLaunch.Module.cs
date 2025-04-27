// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class WindowsLaunch : ModuleRules
{
    public WindowsLaunch()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Launch", "RenderCore");
        AddPrivateAdditionalLibraries("d3d12.lib", "dxgi.lib", "d3d11.lib", "d2d1.lib");
    }
}