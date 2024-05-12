// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class CoreEd : ModuleRules
{
    public CoreEd(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "SlateCore",
            "MainFrame",
            "Engine"
        });
    }
}