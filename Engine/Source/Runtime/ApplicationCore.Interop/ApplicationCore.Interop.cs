using System;
using AE.Rules;

public class ApplicationCore : ModuleRules
{
    public ApplicationCore(TargetInfo targetInfo) : base(targetInfo)
    {
        PublicIncludePaths.Add("Public");
        PrivateIncludePaths.Add("Private");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "CoreAObject.Interop"
        });
    }
}