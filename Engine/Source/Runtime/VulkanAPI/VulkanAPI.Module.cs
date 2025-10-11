// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using System.IO;
using AylaEngine;

public class VulkanAPI : ModuleRules
{
    public VulkanAPI()
    {
        Script.Enabled = true;
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("RenderCore", "ApplicationCore");
        
        if (TargetInfo.Platform.Group == PlatformGroup.Windows)
        {
            string? vulkanSdk = Environment.GetEnvironmentVariable("VULKAN_SDK");
            if (!string.IsNullOrEmpty(vulkanSdk))
            {
                AddPrivateIncludePaths(System.IO.Path.Combine(vulkanSdk, "Include"));

                var libPath = Path.Combine(vulkanSdk, "Lib", "vulkan-1.lib");

                if (!File.Exists(libPath))
                {
                    AddError($"Could not find vulkan-1.lib in the Vulkan SDK directory: {vulkanSdk}. Please ensure the Vulkan SDK is installed correctly.");
                    return;
                }

                AddPrivateAdditionalLibraries(libPath);
            }
            else
            {
                AddError("VULKAN_SDK environment variable is not set. Please install the Vulkan SDK and ensure the environment variable is configured.");
                return;
            }
        }
        else
        {
            AddPrivateAdditionalLibraries("vulkan");
        }
    }
}
