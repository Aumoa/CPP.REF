// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class BlankApp : ModuleRules
{
    public BlankApp(TargetInfo targetInfo) : base(targetInfo)
    {
        Type = ModuleType.ConsoleApplication;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Numerics",
            "CoreAObject"
        });

        PrivateIncludePaths.Add("Private");
    }
}