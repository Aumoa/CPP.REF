// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.SourceTree;

public record SourceDependency
{
    public List<string> IncludeHeaders { get; set; } = new();
}
