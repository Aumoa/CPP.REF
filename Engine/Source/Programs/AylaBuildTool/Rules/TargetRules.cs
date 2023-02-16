// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Rules;

public class TargetRules
{
    public TargetRules(TargetInfo Info)
    {
        Name = GetType().Name.Replace("Target", "");
        TargetName = Info.Name;
        Class = TargetClass.Game;
    }

    public string Name { get; protected set; }

    public string TargetName { get; protected set; }

    public TargetClass Class { get; protected set; }

    public List<string> ExtraModuleNames { get; protected set; } = new();
}
