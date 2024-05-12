// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class SlateRHIRenderer : ModuleRules
{
    public SlateRHIRenderer(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "SlateCore"
        });
    }
}