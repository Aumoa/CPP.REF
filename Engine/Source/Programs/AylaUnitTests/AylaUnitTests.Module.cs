// Copyright 2020-2025 Aumoa.lib. All right reserved.

using AylaEngine;

public class AylaUnitTests : ModuleRules
{
    public AylaUnitTests()
    {
        Script.Enabled = true;

        AddPrivateIncludePaths("Private");
        AddPublicDependencyModuleNames("Core", "Numerics");

        Type = ModuleType.Console;
    }
}
