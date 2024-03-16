// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class ReflectionHeaderWorker : ModuleRules
{
    public ReflectionHeaderWorker(TargetInfo targetInfo) : base(targetInfo)
    {
        Type = ModuleType.ConsoleApplication;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Numerics",
        });

        PrivateIncludePaths.Add("Private");
    }
}