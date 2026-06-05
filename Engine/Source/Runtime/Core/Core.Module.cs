// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Core : ModuleRules
{
    public Core()
    {
        Script.Enabled = true;
        PchUsage = PchUsageMode.UseExplicitOrSharedPCHs;
        PrivatePchHeaderFile = "CoreMinimal.h";

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");

        AddPublicDisableWarnings(4005, 4100, 4251, 4275, 5105);
        AddPrivateDisableWarnings(5106);

        if (TargetInfo.Platform.Group == PlatformGroup.Windows)
        {
            AddPublicAdditionalMacros("WIN32_LEAN_AND_MEAN");
            AddPrivateAdditionalLibraries("dbghelp.lib", "Rpcrt4.lib", "Kernel32.lib", "Ws2_32.lib", "Mswsock.lib");
        }
        else if (TargetInfo.Platform.Group == PlatformGroup.Linux)
        {
            // apt install libicu-dev
            AddPrivateAdditionalLibraries("icuuc", "icudata");
            // apt install liburing-dev
            AddPrivateAdditionalLibraries("uring");
            // apt install libunwind-dev
            AddPublicAdditionalLibraries("unwind");
        }
    }
}
