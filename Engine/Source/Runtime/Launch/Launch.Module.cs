// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Launch : ModuleRules
{
    public Launch()
    {
        Script.Enabled = true;
        Type = ModuleType.Application;
        PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        PrivatePchHeaderFile = "CoreMinimal.h";

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "ApplicationCore", "RenderCore", "Engine");
    }
}
