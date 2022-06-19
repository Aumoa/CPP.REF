// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

record struct SyntaxCore
{
    public SyntaxType Type { get; }

    public string Name { get; }

    public int Line { get; }

    public SyntaxCore(SyntaxType type, string name, int line)
    {
        Type = type;
        Name = name;
        Line = line;
    }
}
