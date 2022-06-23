// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

internal class SyntaxProperty
{
    public AccessModifier AccessModifier { get; set; } = AccessModifier.Private;

    public SyntaxVariable Variable { get; set; } = new();
}
