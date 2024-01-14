// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.CompilerServices;
using AE.CompilerServices.Makefiles;

namespace AE.Platform;

public abstract class Compiler
{
    public abstract Task<string> CompileAsync(MakefileCompile node, CancellationToken cancellationToken = default);

    protected static (string, string)[] GenerateBuildMacros()
    {
        List<(string, string)> macros = new();

        PlatformGroup.ForEach(p =>
        {
            bool isCurrent = p == Target.Info.BuildConfiguration.Platform.Group;
            macros.Add(($"PLATFORM_{p.GroupName.ToUpper()}", isCurrent ? "1" : "0"));
        });

        bool isShipping = Target.Info.BuildConfiguration.Configuration == Configuration.Shipping;
        macros.Add(("SHIPPING", isShipping ? "1" : "0"));

        bool isEditor = Target.IsEditor;
        macros.Add(("WITH_EDITOR", isEditor ? "1" : "0"));

        return macros.ToArray();
    }
}
