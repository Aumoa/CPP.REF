// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class SlateCore : ModuleRules
{
    public SlateCore(TargetRules TargetRule) : base(TargetRule)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Numerics",
            "ApplicationCore"
        });
    }
}