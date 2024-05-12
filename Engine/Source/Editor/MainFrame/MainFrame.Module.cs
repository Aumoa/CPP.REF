// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class MainFrame : ModuleRules
{
    public MainFrame(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "SlateCore",
            "Slate",
            "Engine",
        });
    }
}