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

    public static IEnumerable<TargetRules> GetAllRulesForEditor()
    {
        foreach (var rules in GetAllRulesForEditor_Platforms(TargetPlatform.Win64))
        {
            yield return rules;
        }

        foreach (var rules in GetAllRulesForEditor_Platforms(TargetPlatform.Linux))
        {
            yield return rules;
        }
    }

    public static IEnumerable<TargetRules> GetAllRulesForEditor_Platforms(TargetPlatform platformInfo)
    {
        var targetInfo = new TargetInfo()
        {
            BuildConfiguration = new()
            {
                Configuration = Configuration.Debug,
                Platform = platformInfo
            },
        };

        yield return new TargetRules(targetInfo) { bEditor = false };
        yield return new TargetRules(targetInfo) { bEditor = true };

        targetInfo.BuildConfiguration.Configuration = Configuration.DebugGame;
        yield return new TargetRules(targetInfo) { bEditor = false };
        yield return new TargetRules(targetInfo) { bEditor = true };

        targetInfo.BuildConfiguration.Configuration = Configuration.Development;
        yield return new TargetRules(targetInfo) { bEditor = false };
        yield return new TargetRules(targetInfo) { bEditor = true };

        targetInfo.BuildConfiguration.Configuration = Configuration.Shipping;
        yield return new TargetRules(targetInfo) { bEditor = false };
    }
}
