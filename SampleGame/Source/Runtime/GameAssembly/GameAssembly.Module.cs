// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class GameAssembly : ModuleRules
{
    public GameAssembly()
    {
        Script.Enabled = true;
        Type = ModuleType.Game;
        PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        PrivatePchHeaderFile = "CoreMinimal.h";

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Numerics", "RenderCore", "Engine");
    }
}
