// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Launch : ModuleRules
{
    public Launch()
    {
        Type = ModuleType.Application;

        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "RHI", "SlateCore", "SlateRHIRenderer", "Engine", "Slate", "CoreEd", "MainFrame");
    }
}