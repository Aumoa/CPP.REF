// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

using BuildTool;

public class jsonRule : ModuleRule
{
    public jsonRule()
    {
        Category = ModuleCategory.ThirdParty;
        FilterPath = "ThirdParty";
        ModuleType = ModuleType.None;

        PublicIncludePaths.Add("include");
        PrivateIncludePaths.Add("src");
    }
}