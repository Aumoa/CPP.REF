// Copyright 2020-2021 Aumoa.lib. All right reserved.

public class GameBuild : BuildRules
{
    public GameBuild()
    {
        CppStandardVersion = CppVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Assimp",
            "FbxSDK",
        });

        AdditionalDependencies.AddRange(new string[]
        {
            "DXGI.lib",
            "D3D12.lib",
            "libfbxsdk-md.lib"
        });
    }
}