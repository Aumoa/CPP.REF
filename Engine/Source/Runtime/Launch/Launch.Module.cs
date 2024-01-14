// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class Launch : ModuleRules
{
    public Launch(TargetInfo targetInfo) : base(targetInfo)
    {
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