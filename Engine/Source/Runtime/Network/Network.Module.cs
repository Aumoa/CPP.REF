// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using BuildTool;

public class NetworkRule : ModuleRule
{
    public NetworkRule()
    {
        Category = ModuleCategory.Engine;
        FilterPath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");
        SupportsVcpkg = false;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core"
        });

        PrivateDependencyModuleNames.AddRange(new[]
        {
            "json"
        });
    }
}