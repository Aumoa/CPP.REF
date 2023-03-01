// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;
using AE.BuildSettings;
using AE.Platform;

public class ApplicationCore : ModuleRules
{
    public ApplicationCore(TargetRules TargetRule) : base(TargetRule)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Numerics"
        });

        if (TargetRule.Platform.Group == PlatformGroup.Windows)
        {
            if (TargetRule.Platform.Architecture == Architecture.x64)
            {
                PrivateAdditionalMacros.AddRange(new[]
                {
                    "_WIN64"
                });
            }

            PrivateAdditionalLibraries.AddRange(new[]
            {
                "gdiplus.lib"
            });
        }
    }
}