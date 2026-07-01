// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using System.IO;
using AylaEngine;

public class ShaderCompileWorker : ModuleRules
{
    public ShaderCompileWorker()
    {
        Type = ModuleType.Console;
        PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        PrivatePchHeaderFile = "CoreMinimal.h";

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core");

        string? vulkanSdk = Environment.GetEnvironmentVariable("VULKAN_SDK");
        if (string.IsNullOrEmpty(vulkanSdk))
        {
            AddError("VULKAN_SDK environment variable is not set. Please install the Vulkan SDK and ensure the environment variable is configured.");
            return;
        }

        var includePath = Path.Combine(vulkanSdk, "Include", "dxc");
        if (!Directory.Exists(includePath))
        {
            AddError($"Could not find the Include directory in the Vulkan SDK directory: {vulkanSdk}. Please ensure the Vulkan SDK is installed correctly.");
            return;
        }

        var libPath = Path.Combine(vulkanSdk, "Lib", "dxcompiler.lib");
        if (!File.Exists(libPath))
        {
            AddError($"Could not find dxcompiler.lib in the Vulkan SDK directory: {vulkanSdk}. Please ensure the Vulkan SDK is installed correctly.");
            return;
        }

        AddPrivateIncludePaths(includePath);
        AddPrivateAdditionalLibraries(libPath);
    }
}
