// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Rules;
using AE.SourceTree;

namespace AE.Platform;

public abstract class Compiler
{
    public abstract Task<string> CompileAsync(CompileNode Node, TargetRules Rule, CancellationToken SToken = default);

    protected static (string, string)[] GenerateBuildMacros(TargetRules Rule)
    {
        List<(string, string)> Macros = new();

        PlatformGroup.ForEach(p =>
        {
            bool bCurrent = p == Rule.Target.BuildConfiguration.Platform.Group;
            Macros.Add(($"PLATFORM_{p.GroupName.ToUpper()}", bCurrent ? "1" : "0"));
        });

        bool bShipping = Rule.Target.BuildConfiguration.Configuration == Configuration.Shipping;
        Macros.Add(("SHIPPING", bShipping ? "1" : "0"));

        bool bEditor = Rule.bEditor;
        Macros.Add(("WITH_EDITOR", bEditor ? "1" : "0"));

        return Macros.ToArray();
    }
}
