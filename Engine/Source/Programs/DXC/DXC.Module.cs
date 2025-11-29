// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class DXC : ModuleRules
{
    public DXC()
    {
        Script.Enabled = true;
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core");
        Type = ModuleType.Application;
    }
}