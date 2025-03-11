// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class ReflectionHeaderWorker : ModuleRules
{
    public ReflectionHeaderWorker()
    {
        AddPublicDependencyModuleNames("Core", "Numerics");
        AddPrivateIncludePaths("Private");
    }
}