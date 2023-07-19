// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class BlankAppTarget : TargetRules
{
    public BlankAppTarget(TargetInfo Info) : base(Info)
    {
        Type = TargetType.ConsoleApplication;
    }
}