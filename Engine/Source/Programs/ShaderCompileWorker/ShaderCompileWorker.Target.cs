// Copyright 2020-2021 Aumoa.lib. All right reserved.

using System;
using AE.Rules;

public class ShaderCompileWorkerTarget : TargetRules
{
    public ShaderCompileWorkerTarget(TargetInfo Info) : base(Info)
    {
        Type = TargetType.ConsoleApplication;
    }
}