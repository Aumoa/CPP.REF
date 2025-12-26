// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class DXC : ModuleRules
{
    public DXC()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core");
        Type = ModuleType.Console;

        // TODO: Parse version string and select latest.
        AddPrivateIncludePaths("C:\\VulkanSDK\\1.4.313.2\\Include\\dxc");
        AddPrivateAdditionalLibraries("C:\\VulkanSDK\\1.4.313.2\\Lib\\dxcompiler.lib");
    }
}