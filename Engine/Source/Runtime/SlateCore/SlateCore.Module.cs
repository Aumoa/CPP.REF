// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class SlateCore : ModuleRules
{
    public SlateCore()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Numerics", "ApplicationCore", "RenderCore");
    }
}