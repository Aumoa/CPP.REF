// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Core : ModuleRules
{
    public Core()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");

        AddPublicDisableWarnings(4251, 4275, 5105, 4005);
        AddPrivateDisableWarnings(5106);

        if (TargetInfo.Platform.Group == PlatformGroup.Windows)
        {
            AddPublicAdditionalMacros("NOMINMAX", "WIN32_LEAN_AND_MEAN");
            AddPrivateAdditionalLibraries("dbghelp.lib", "Rpcrt4.lib", "Kernel32.lib", "Ws2_32.lib", "winsock2.lib");
        }
    }
}