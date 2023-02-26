// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;

using System.Diagnostics;

namespace AE.Rules;

public class TargetRules
{
    public TargetRules(TargetInfo Info)
    {
        Name = GetType().Name.Replace("Target", "");
        Debug.Assert(Info.Name != null);
        TargetName = Info.Name;
        Class = TargetClass.Game;
        Platform = Info.BuildConfiguration.Platform;
        TargetModuleName = Name;
    }

    public string Name { get; protected set; }

    public string TargetName { get; protected set; }

    public TargetClass Class { get; protected set; }

    public TargetType Type { get; protected set; }

    public TargetPlatform Platform { get; protected set; }

    public string TargetModuleName { get; protected set; }

    public bool bBuildAllModules { get; set; }
}
