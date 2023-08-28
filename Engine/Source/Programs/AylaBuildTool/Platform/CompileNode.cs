// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.SourceTree;

namespace AE.Platform;

public record CompileNode
{
    public required MakefileCompile Compile { get; init; }
}
