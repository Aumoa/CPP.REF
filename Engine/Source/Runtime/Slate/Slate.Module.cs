// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Slate : ModuleRules
{
    public Slate()
    {
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "SlateCore");
    }
}