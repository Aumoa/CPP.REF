// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Core : ModuleRules
{
    public Core()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");

        AddPublicDisableWarnings(4005, 4100, 4251, 4275, 5105);
        AddPrivateDisableWarnings(5106);

        EnableScript = true;
        DisableGenerateBindings = true;

        if (TargetInfo.Platform.Group == PlatformGroup.Windows)
        {
            AddPublicAdditionalMacros("WIN32_LEAN_AND_MEAN");
            AddPrivateAdditionalLibraries("dbghelp.lib", "Rpcrt4.lib", "Kernel32.lib", "Ws2_32.lib");
        }
    }
}