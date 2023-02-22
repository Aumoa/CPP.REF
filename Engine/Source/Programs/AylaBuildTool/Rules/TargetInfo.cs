// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;

namespace AE.Rules;

public record TargetInfo
{
    public string Name { get; set; } = null!;

    public BuildConfiguration BuildConfiguration { get; set; }
}
