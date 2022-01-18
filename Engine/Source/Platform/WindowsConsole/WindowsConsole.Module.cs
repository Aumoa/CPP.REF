// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class WindowsConsole : ModuleRule
{
    public WindowsConsole()
    {
        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            TargetType = TargetType.Engine;
            ModuleType = ModuleType.ConsoleApplication;

            RelativePath = "Engine.Platform";
            TargetName = "$(SolutionName)";

            PrivateIncludePaths.Add("Private");

            PublicDependencyModuleNames.AddRange(new[]
            {
                "Core",
                "Console",
                "WindowsCommon"
            });
        }
        else
        {
            TargetType = TargetType.None;
            ModuleType = ModuleType.None;
        }
    }
}