using System;
using AE.Rules;

public class CoreAObject : ModuleRules
{
    public CoreAObject(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");
    }
}