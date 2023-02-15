// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;

using BuildTool.Rule;

public class mysql : ModuleRule
{
    public mysql(IReadonlyTargetRules targets) : base(targets)
    {
        Category = ModuleCategory.ThirdParty;
        Type = ModuleType.Misc;

        PublicIncludePaths.Add("include");
        PublicAdditionalLibraries.AddRange(new[]
        {
            "lib/vs14/mysqlcppconn.lib"
        });
    }
}