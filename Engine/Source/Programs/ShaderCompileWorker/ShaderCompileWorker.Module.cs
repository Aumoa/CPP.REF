// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AylaEngine;

public class ShaderCompileWorker : ModuleRules
{
    public ShaderCompileWorker()
    {
        AddPublicDependencyModuleNames("Core");
        AddPrivateIncludePaths("Private");

        if (TargetInfo.Platform.Group == PlatformGroup.Windows)
        {
            AddPrivateAdditionalLibraries("dxcompiler.lib");
        }
    }
}