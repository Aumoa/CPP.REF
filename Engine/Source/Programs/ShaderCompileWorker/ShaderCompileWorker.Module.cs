// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class ShaderCompileWorker : ModuleRules
{
    public ShaderCompileWorker(TargetRules TargetRule) : base(TargetRule)
    {
        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core"
        });

        PrivateIncludePaths.Add("Private");
    }
}