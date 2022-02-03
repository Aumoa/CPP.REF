// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using CodeProjectConfiguration;

public class DirectX : ModuleRule
{
    public DirectX()
    {
        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            //TargetType = TargetType.ThirdParty;
            TargetType = TargetType.Engine;
            ModuleType = ModuleType.GameModule;
            RelativePath = "ThirdParty";

            PublicIncludePaths.Add("Public");
            PrivateIncludePaths.Add("Private");

            PublicDependencyModuleNames.AddRange(new[]
            {
                "Core",
                "PlatformGeneric",
                "WindowsCommon",
            });

            PrivateAdditionalLibraries.AddRange(new[]
            {
                "DXGI.lib",
                "D3D12.lib",
                "D3D11.lib",
                "D2D1.lib",
                "DWrite.lib",
                "D3DCompiler.lib"
            });
        }
        else
        {
            // The module available only Windows Subsystem.
            TargetType = TargetType.None;
            ModuleType = ModuleType.None;
        }
    }
}