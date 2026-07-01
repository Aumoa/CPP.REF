// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using System.IO;
using AylaEngine;

public class Direct3D12 : ModuleRules
{
    public Direct3D12()
    {
        Script.Enabled = true;
        PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        PrivatePchHeaderFile = "CoreMinimal.h";

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("RenderCore", "ApplicationCore");
        AddPrivateAdditionalLibraries("dxgi.lib", "d3d12.lib");
    }
}
