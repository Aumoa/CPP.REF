using System;
using AylaEngine;

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
                "ApplicationCore.Interop"
            });
        }
    }
}