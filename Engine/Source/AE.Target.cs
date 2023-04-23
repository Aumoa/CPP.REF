// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class AETarget : TargetRules
{
    public AETarget(TargetInfo Info) : base(Info)
    {
        TargetModuleName = "Core";
        Type = TargetType.Module;
    }
}