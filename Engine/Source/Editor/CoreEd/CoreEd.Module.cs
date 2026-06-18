// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class CoreEd : ModuleRules
{
    public CoreEd()
    {
        PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        PrivatePchHeaderFile = "CoreMinimal.h";

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Engine", "ApplicationCore");
    }
}
