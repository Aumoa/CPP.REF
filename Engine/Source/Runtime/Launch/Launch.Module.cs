// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Launch : ModuleRules
{
    public Launch()
    {
        Scriptable.Enabled = true;
        Scriptable.NuGetPackages.Add("CommandLineParser, Version=2.9.1");
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "ApplicationCore", "RenderCore", "Engine");
        Type = ModuleType.Application;
    }
}