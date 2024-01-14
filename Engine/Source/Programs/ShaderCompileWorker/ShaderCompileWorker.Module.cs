// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;
using AE.BuildSettings;

public class ShaderCompileWorker : ModuleRules
{
    public ShaderCompileWorker(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core"
        });

        PrivateIncludePaths.Add("Private");

        if (targetInfo.Platform.Group == PlatformGroup.Windows)
        {
            PrivateAdditionalLibraries.Add("dxcompiler.lib");
        }
    }
}