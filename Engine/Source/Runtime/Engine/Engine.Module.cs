// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Engine : ModuleRules
{
    public Engine()
    {
        Script.Enabled = true;
        PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        PrivatePchHeaderFile = "CoreMinimal.h";

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Numerics", "RenderCore");
    }
}
