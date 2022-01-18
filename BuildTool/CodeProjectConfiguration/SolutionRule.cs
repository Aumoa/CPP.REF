// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace CodeProjectConfiguration;

public class SolutionRule
{
    public SolutionRule()
    {
        Name = GetType().Name;
    }

    public string Name;
    public string EngineRoot;
    public string ThirdPartyRoot;
    public string PrimaryModule;
}