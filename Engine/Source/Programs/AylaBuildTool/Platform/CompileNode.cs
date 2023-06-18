// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.SourceTree;

namespace AE.Platform;

public record CompileNode
{
    public required MakefileCompile Compile { get; init; }

    public required HashSet<string> IfcSearchDirs { get; init; }

    public required HashSet<string> HeaderImports { get; init; }

    public required HashSet<CompileNode> Parents { get; init; }

    public required TaskCompletionSource CompileTask { get; init; }
}
