// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Misc;

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace AE.Rules;

public class TargetRules : SerializableRule
{
    [SetsRequiredMembers]
    public TargetRules(TargetInfo Info)
    {
        Target = Info;
        Name = GetType().Name.Replace("Target", "");
        Class = TargetClass.Game;
        Platform = Info.BuildConfiguration.Platform;
        TargetModuleName = GetType().Name.Replace("Target", "");
    }

    public required TargetInfo Target { get; init; }

    public required string Name { get; init; }

    public TargetClass Class { get; protected set; }

    public TargetType Type { get; protected set; }

    public TargetPlatform Platform { get; protected set; }

    public string TargetModuleName { get; protected set; }
}
