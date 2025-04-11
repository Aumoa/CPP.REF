// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Engine : ModuleRules
{
    public Engine()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Numerics");

        EnableScript = true;
    }
}