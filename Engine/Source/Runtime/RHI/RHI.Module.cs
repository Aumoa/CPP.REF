// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;
using AE.BuildSettings;

public class RHI : ModuleRules
{
    public RHI(TargetRules TargetRule) : base(TargetRule)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core"
        });

        if (TargetRule.Platform.Group == PlatformGroup.Windows)
        {
            PrivateAdditionalLibraries.AddRange(new[]
            {
                "dxgi.lib",
                "d3d12.lib"
            });
        }
    }
}