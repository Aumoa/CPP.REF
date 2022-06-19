// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

internal record SyntaxConstructor
{
    public List<string> Arguments { get; set; } = null!;

    public string SafeName => string
        .Concat(Arguments)
        .Replace(" ", "_1_")
        .Replace(",", "_2_")
        .Replace("&", "_3_")
        .Replace("*", "_4_");
}
