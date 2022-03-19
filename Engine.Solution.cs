// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

public class Engine : SolutionRule
{
    public Engine()
    {
        EngineRoot = "./Engine";
        ThirdPartyRoot = "./ThirdParty";
        PrimaryModule = "Game";
    }
}