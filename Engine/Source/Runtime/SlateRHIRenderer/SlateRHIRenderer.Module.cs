﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;
using AE.BuildSettings;

public class SlateRHIRenderer : ModuleRules
{
    public SlateRHIRenderer(TargetRules TargetRule) : base(TargetRule)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "SlateCore"
        });

        PrivateDisableWarnings.Add(4324);
    }
}