// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

using BuildTool.Rule;

public class json : ModuleRule
{
    public json(IReadonlyTargetRules targets) : base(targets)
    {
        Category = ModuleCategory.ThirdParty;
        Type = ModuleType.Misc;

        PublicIncludePaths.Add("include");
        PrivateIncludePaths.Add("src");
    }
}