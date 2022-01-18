// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

public class Core : ModuleRule
{
    public Core()
    {
        TargetType = TargetType.Engine;

        RelativePath = "Engine.Runtime";

        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDisableWarnings.AddRange(new[] {
            4250,
            4251,
            5105,
            4275,
            4005,
        });

        PrivatePreprocessorDefines.Add("NOMINMAX");
        PublicDependencyModuleNames.Add("Boost.Asio");
    }
}