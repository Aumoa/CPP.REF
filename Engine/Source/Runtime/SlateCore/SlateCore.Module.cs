// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class SlateCore : ModuleRules
{
    public SlateCore(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Numerics",
            "ApplicationCore",
            "RenderCore",
        });
    }
}