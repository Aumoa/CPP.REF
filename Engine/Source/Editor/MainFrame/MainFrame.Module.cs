// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class MainFrame : ModuleRules
{
    public MainFrame()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "SlateCore", "Slate", "Engine");
    }
}