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
        TargetModuleName = Name;
    }

    internal TargetRules(TargetInfo Info, string Name)
    {
        Target = Info;
        this.Name = Name;
        Platform = Info.BuildConfiguration.Platform;
        TargetModuleName = Name;
    }

    public TargetInfo Target { get; }

    public string Name { get; }

    public TargetType Type { get; set; }

    public TargetPlatform Platform { get; set; }

    public string TargetModuleName { get; set; }

    public bool bEditor { get; set; } = false;
}
