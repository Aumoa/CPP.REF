// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace CodeProjectConfiguration.Boost;

using System;

public class Asio : ModuleRule
{
    public Asio()
    {
        Name = "Boost.Asio";
        TargetName = "boost-asio";
        RelativePath = "ThirdParty";
        TargetType = TargetType.ThirdParty;
        ModuleType = ModuleType.Vcpkg;

        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            PublicPreprocessorDefines.AddRange(new[]
            {
                "_WIN32_WINNT=0x0A00"
            });
        }
    }
}