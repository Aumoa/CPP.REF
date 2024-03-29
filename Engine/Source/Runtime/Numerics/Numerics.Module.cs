﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class Numerics : ModuleRules
{
    public Numerics(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core"
        });

        PublicDisableWarnings.AddRange(new[]
        {
            // In C11, unnamed union structure is allowed actually.
            4201
        });
    }
}