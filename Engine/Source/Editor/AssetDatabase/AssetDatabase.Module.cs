// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class AssetDatabase : ModuleRules
{
    public AssetDatabase()
    {
        PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        PrivatePchHeaderFile = "CoreMinimal.h";

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("CoreEd");
    }
}
