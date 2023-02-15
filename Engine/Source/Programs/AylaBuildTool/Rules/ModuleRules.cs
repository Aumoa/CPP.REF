// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Rules;

public class ModuleRules
{
    public TargetRules TargetRule { get; }

    public ModuleRules(TargetRules TargetRule)
    {
        this.TargetRule = TargetRule;
    }
}
