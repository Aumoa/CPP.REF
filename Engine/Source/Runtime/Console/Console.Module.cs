// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using BuildTool;

public class ConsoleRule : ModuleRule
{
    public ConsoleRule()
    {
        Category = ModuleCategory.Engine;
        FilterPath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PrivateAdditionalMacros.AddRange(new[]
            {
                "NOMINMAX",
                "WIN32_LEAN_AND_MEAN"
            });

            PublicAdditionalLibraries.AddRange(new[]
            {
                "psapi.lib",
                "dbghelp.lib"
            });

            PrivateDisableWarnings.AddRange(new[]
            {
                5105,
                4005,
                5106,
            });
        }
    }
}