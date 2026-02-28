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
        AddPublicIncludePaths("C:\\VulkanSDK\\1.4.335.0\\Include");
        AddPublicDependencyModuleNames("RenderCore", "ApplicationCore");
        
        if (TargetInfo.Platform.Group == PlatformGroup.Windows)
        {
            string? vulkanSdk = Environment.GetEnvironmentVariable("VULKAN_SDK");
            if (!string.IsNullOrEmpty(vulkanSdk))
            {
                var includePath = Path.Combine(vulkanSdk, "Include");
                if (!Directory.Exists(includePath))
                {
                    AddError($"Could not find the Include directory in the Vulkan SDK directory: {vulkanSdk}. Please ensure the Vulkan SDK is installed correctly.");
                    return;
                }

                AddPrivateIncludePaths(includePath);

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
