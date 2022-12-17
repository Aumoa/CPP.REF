// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

using BuildTool;

public class mysqlRule : ModuleRule
{
    public mysqlRule()
    {
        Category = ModuleCategory.ThirdParty;
        FilterPath = "ThirdParty";
        ModuleType = ModuleType.None;

        PublicIncludePaths.Add("include");
    }
}