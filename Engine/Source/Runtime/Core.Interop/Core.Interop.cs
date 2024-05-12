using System;
using AylaEngine;

public class Core : ModuleRules
{
    public Core(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");
    }
}