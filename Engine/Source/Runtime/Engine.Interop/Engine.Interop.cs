﻿using System;
using AE.Rules;

public class Engine : ModuleRules
{
    public Engine(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "CoreAObject"
        });
    }
}