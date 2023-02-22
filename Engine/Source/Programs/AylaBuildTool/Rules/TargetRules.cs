// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;

namespace AE.Rules;

public class TargetRules
{
    public TargetRules(TargetInfo Info)
    {
        Name = GetType().Name.Replace("Target", "");
        TargetName = Info.Name;
        Class = TargetClass.Game;
        Platform = Info.BuildConfiguration.Platform;
    }

    public string Name { get; protected set; }

    public string TargetName { get; protected set; }

    public TargetClass Class { get; protected set; }

    public TargetType Type { get; protected set; }

    public TargetPlatform Platform { get; protected set; }

    public List<string> ExtraModuleNames { get; protected set; } = new();
}
