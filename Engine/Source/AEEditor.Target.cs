// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class AEEditorTarget : TargetRules
{
    public AEEditorTarget(TargetInfo Info) : base(Info)
    {
        TargetModuleName = "Launch";
        Type = TargetType.SlateApplication;
        bEditor = true;
    }
}