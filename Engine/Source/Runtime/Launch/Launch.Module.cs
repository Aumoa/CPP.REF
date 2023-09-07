// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class Launch : ModuleRules
{
    public Launch(TargetRules TargetRule) : base(TargetRule)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "RHI",
            "SlateCore",
            "CoreAObject",
            "SlateRHIRenderer"
        });
    }
}