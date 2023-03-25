﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;
using AE.BuildSettings;

public class Sockets : ModuleRules
{
    public Sockets(TargetRules TargetRule) : base(TargetRule)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

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

            PublicAdditionalLibraries.AddRange(new[]
            {
                "ws2_32.lib"
            });
        }
    }
}