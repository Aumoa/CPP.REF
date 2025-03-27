// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class WindowsLaunch : ModuleRules
{
    public WindowsLaunch()
    {
        Type = ModuleType.Application;
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Launch");
    }
}