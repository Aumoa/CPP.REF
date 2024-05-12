using System;
using AylaEngine;

public class Engine : ModuleRules
{
    public Engine(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core.Interop"
        });
    }
}