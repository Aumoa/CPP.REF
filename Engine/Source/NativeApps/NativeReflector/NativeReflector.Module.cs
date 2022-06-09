// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using BuildTool;

public class NativeReflectorRule : ModuleRule
{
    public NativeReflectorRule()
    {
        Category = ModuleCategory.Engine;
        FilterPath = "Engine.NativeApps";

        ModuleType = ModuleType.ConsoleApplication;

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "Console",
        });
    }
}