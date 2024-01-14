// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;
using AE.BuildSettings;
using AE.Platform;

public class ApplicationCore : ModuleRules
{
    public ApplicationCore(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Numerics"
        });

        if (targetInfo.Platform.Group == PlatformGroup.Windows)
        {
            if (targetInfo.Platform.Architecture == Architecture.x64)
            {
                PrivateAdditionalMacros.AddRange(new[]
                {
                    "_WIN64"
                });
            }

            PrivateAdditionalLibraries.AddRange(new[]
            {
                "Gdi32.lib",
                "gdiplus.lib"
            });

            PrivateDisableWarnings.AddRange(new[]
            {
                4245,
                4458,
            });
        }
    }
}