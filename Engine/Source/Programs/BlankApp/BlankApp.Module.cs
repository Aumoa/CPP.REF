// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class BlankApp : ModuleRules
{
    public BlankApp(TargetRules TargetRule) : base(TargetRule)
    {
        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Numerics"
        });

        PrivateIncludePaths.Add("Private");
    }
}