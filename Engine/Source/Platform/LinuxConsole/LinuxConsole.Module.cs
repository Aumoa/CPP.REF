// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class LinuxConsole : ModuleRule
{
    public LinuxConsole()
    {
        TargetType = TargetType.Engine;
        ModuleType = ModuleType.ConsoleApplication;
        NonUnityBuild = true;
        RelativePath = "Engine.Platform";
        TargetName = "$(SolutionName)";

        PrivateIncludePaths.Add("Private");
        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreSObject",
            "Console",
            "LinuxCommon"
        });

        if (Environment.OSVersion.Platform == PlatformID.Unix)
        {
        }
        else
        {
            ModuleType = ModuleType.None;
        }
    }
}