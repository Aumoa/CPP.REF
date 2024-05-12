using System;
using AE.Rules;

public class Core : ModuleRules
{
    public Core(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");
    }
}