﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class RenderCore : ModuleRules
{
    public RenderCore()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Numerics", "RHI");
    }
}