// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

using BuildTool;

public class amqpcppRule : ModuleRule
{
    public amqpcppRule()
    {
        Category = ModuleCategory.ThirdParty;
        FilterPath = "ThirdParty";
        ModuleType = ModuleType.None;

        PublicIncludePaths.Add("include");
    }
}