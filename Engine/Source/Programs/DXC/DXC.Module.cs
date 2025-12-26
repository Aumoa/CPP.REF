// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class DXC : ModuleRules
{
    public DXC()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core");
        Type = ModuleType.Console;

        AddPrivateAdditionalLibraries("dxcompiler.lib");
    }
}