// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using System;

public class LinuxCommon : ModuleRule
{
    public LinuxCommon()
    {
        TargetType = TargetType.Engine;
        NonUnityBuild = true;
        RelativePath = "Engine.Platform";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");
        PublicDependencyModuleNames.Add("Core");

        if (Environment.OSVersion.Platform == PlatformID.Unix)
        {
        }
        else
        {
            ModuleType = ModuleType.None;
        }
    }
}