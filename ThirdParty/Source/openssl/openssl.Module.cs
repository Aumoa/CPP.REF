// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

public class OpenSSL : ModuleRule
{
    public OpenSSL()
    {
        TargetType = TargetType.ThirdParty;
        ModuleType = ModuleType.Vcpkg;
        RelativePath = "ThirdParty";
    }
}