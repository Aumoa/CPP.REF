// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

using BuildTool.Rule;

public class amqpcpp : ModuleRule
{
    public amqpcpp(IReadonlyTargetRules targets) : base(targets)
    {
        Category = ModuleCategory.ThirdParty;
        Type = ModuleType.Misc;

        PublicIncludePaths.Add("include");
    }
}