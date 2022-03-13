// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class WindowsCommon : ModuleRule
{
    public WindowsCommon()
    {
        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        { 
            TargetType = TargetType.Engine;
            NonUnityBuild = true;
            RelativePath = "Engine.Platform";

            PublicIncludePaths.Add("Public");
            PrivateIncludePaths.Add("Private");

            PublicDependencyModuleNames.Add("Core");

            PrivateAdditionalLibraries.AddRange(new[]
            {
                "psapi.lib",
                "dbghelp.lib"
            });
        }
        else
        {
            TargetType = TargetType.None;
            ModuleType = ModuleType.None;
        }
    }
}