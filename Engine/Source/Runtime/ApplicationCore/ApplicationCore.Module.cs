// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class ApplicationCore : ModuleRules
{
    public ApplicationCore()
    {
        // ApplicationCore must not be scriptable to support pure native console applications
        Script.Enabled = false;
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Numerics");
    }
}