﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class CoreAObject : ModuleRules
{
    public CoreAObject(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core"
        });
    }
}