// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class Launch : ModuleRules
{
    public Launch(TargetInfo targetInfo) : base(targetInfo)
    {
        Type = ModuleType.Application;

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "RHI",
            "SlateCore",
            "SlateRHIRenderer",
            "Engine",
            "Slate",
            "CoreEd",
            "MainFrame"
        });
    }
}