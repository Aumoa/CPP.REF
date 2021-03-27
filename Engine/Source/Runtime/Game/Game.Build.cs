// Copyright 2020-2021 Aumoa.lib. All right reserved.

public class GameBuild : BuildRules
{
    public GameBuild()
    {
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "CoreSObject"
        });

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core"
        });
    }
}