using System;
using AE.Rules;

namespace Windows
{
    public class Launch : ModuleRules
    {
        public Launch(TargetInfo targetInfo) : base(targetInfo)
        {
            Type = ModuleType.Application;

            PublicIncludePaths.Add("Public");
            PrivateIncludePaths.Add("Private");

            PublicDependencyModuleNames.AddRange(new[]
            {
                "CoreAObject.Interop",
                "ApplicationCore.Interop"
            });
        }
    }
}