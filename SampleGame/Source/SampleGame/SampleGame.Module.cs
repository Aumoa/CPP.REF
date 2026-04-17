using AylaEngine;

public class SampleGame : ModuleRules
{
    public SampleGame()
    {
        Type = ModuleType.Game;
        AddPublicIncludePaths("Public");
        AddPrivateIncludePaths("Private");
        AddPrivateDependencyModuleNames("Core", "Engine");
    }
}