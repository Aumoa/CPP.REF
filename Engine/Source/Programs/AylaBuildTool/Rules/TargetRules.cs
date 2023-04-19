// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Misc;

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace AE.Rules;

public class TargetRules
{
    public TargetRules(TargetInfo Info)
    {
        Target = Info;
        Name = GetType().Name.Replace("Target", "");
        Platform = Info.BuildConfiguration.Platform;
        TargetModuleName = GetType().Name.Replace("Target", "");
    }

    public TargetInfo Target { get; }

    public string Name { get; }

    public TargetType Type { get; protected set; }

    public TargetPlatform Platform { get; protected set; }

    public string TargetModuleName { get; protected set; }

    public bool bEditor { get; protected set; } = false;
}
