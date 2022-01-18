// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

public class TinyXml2 : ModuleRule
{
    public TinyXml2()
    {
        TargetType = TargetType.ThirdParty;
        ModuleType = ModuleType.Vcpkg;
        RelativePath = "ThirdParty";
    }
}