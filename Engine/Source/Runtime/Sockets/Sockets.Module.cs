// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class Sockets : ModuleRule
{
    public Sockets()
    {
        TargetType = TargetType.Engine;

        RelativePath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PrivatePreprocessorDefines.AddRange(new[]
            {
                "NOMINMAX",
                "WIN32_LEAN_AND_MEAN",
                "_WINSOCK_DEPRECATED_NO_WARNINGS",
            });

            PrivateAdditionalLibraries.AddRange(new[]
            {
                "Ws2_32.lib"
            });
        }

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core"
        });
    }
}