// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;

namespace AE.Rules;

public class TargetInfo : SerializableRule
{
    public BuildConfiguration BuildConfiguration { get; set; } = new();

    public bool bEditor { get; set; }
}
