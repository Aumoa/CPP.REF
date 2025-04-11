// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class WindowsLaunch : ModuleRules
{
    public WindowsLaunch()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Launch");
        EnableScript = true;
    }
}