// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;
using AE.BuildSettings;

public class Core : ModuleRules
{
    public Core(TargetRules TargetRule) : base(TargetRule)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDisableWarnings.AddRange(new[]
        {
            4251,
            4275,
            5105,
            4005,
        });

        PrivateDisableWarnings.AddRange(new[]
        {
            5106,
        });

        if (TargetRule.Platform.Group == PlatformGroup.Windows)
        {
            PublicAdditionalMacros.AddRange(new[]
            {
                "NOMINMAX",
                "WIN32_LEAN_AND_MEAN"
            });

            PrivateAdditionalLibraries.AddRange(new[]
            {
                "dbghelp.lib",
                "Rpcrt4.lib",
                "Kernel32.lib"
            });
        }
    }
}