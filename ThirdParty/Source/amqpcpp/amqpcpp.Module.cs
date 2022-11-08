// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

using BuildTool;

public class amqpcppRule : ModuleRule
{
    public amqpcppRule()
    {
        Category = ModuleCategory.ThirdParty;
        FilterPath = "ThirdParty";

        PublicIncludePaths.Add("include");
        PrivateIncludePaths.Add("src");

        PublicAdditionalMacros.AddRange(new[]
        {
            "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING"
        });

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PublicAdditionalMacros.AddRange(new[]
            {
                "NOMINMAX"
            });
        }
    }
}