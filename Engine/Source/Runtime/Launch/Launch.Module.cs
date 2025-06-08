// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Launch : ModuleRules
{
    public Launch()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "ApplicationCore", "Engine");
        Type = ModuleType.Application;

        if (TargetInfo.Editor)
        {
            AddPublicDependencyModuleNames("CoreEd");
        }
    }
}