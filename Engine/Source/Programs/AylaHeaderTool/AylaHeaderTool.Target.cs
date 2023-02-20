﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class AylaHeaderToolTarget : TargetRules
{
    public AylaHeaderToolTarget(TargetInfo Info) : base(Info)
    {
        Class = TargetClass.Program;
        Type = TargetType.ConsoleApplication;
        ExtraModuleNames.Add("AylaHeaderTool");
    }
}