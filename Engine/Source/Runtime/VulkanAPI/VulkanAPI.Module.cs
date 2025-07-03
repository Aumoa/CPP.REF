// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class VulkanAPI : ModuleRules
{
    public VulkanAPI()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("RenderCore", "ApplicationCore");
        AddPrivateAdditionalLibraries("vulkan");
    }
}
