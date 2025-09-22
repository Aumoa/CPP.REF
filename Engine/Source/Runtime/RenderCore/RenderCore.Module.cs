// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class RenderCore : ModuleRules
{
    public RenderCore()
    {
        Script.Enabled = true;
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Numerics", "ApplicationCore");
    }
}