// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Numerics : ModuleRules
{
    public Numerics()
    {
        Script.Enabled = true;
        PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        PrivatePchHeaderFile = "CoreMinimal.h";

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core");

        // In C11, unnamed union structure is allowed actually.
        AddPublicDisableWarnings(4201, 5063);
    }
}
