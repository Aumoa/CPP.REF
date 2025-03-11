// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class ApplicationCore : ModuleRules
{
    public ApplicationCore()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Numerics");

        if (TargetInfo.Platform.Group == PlatformGroup.Windows)
        {
            if (TargetInfo.Platform.Architecture == Architecture.X64)
            {
                AddPrivateAdditionalMacros("_WIN64");
            }

            AddPrivateAdditionalLibraries("Gdi32.lib", "gdiplus.lib");
            AddPrivateDisableWarnings(4245, 4458);
        }
    }
}