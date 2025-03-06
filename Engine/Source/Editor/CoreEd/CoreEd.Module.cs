// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class CoreEd : ModuleRules
{
    public CoreEd()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "SlateCore", "MainFrame", "Engine");
    }
}