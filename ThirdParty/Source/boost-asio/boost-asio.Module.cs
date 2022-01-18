// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace CodeProjectConfiguration.Boost;

public class Asio : ModuleRule
{
    public Asio()
    {
        Name = "Boost.Asio";
        TargetName = "boost-asio";
        RelativePath = "ThirdParty";
        TargetType = TargetType.ThirdParty;
        ModuleType = ModuleType.Vcpkg;
    }
}