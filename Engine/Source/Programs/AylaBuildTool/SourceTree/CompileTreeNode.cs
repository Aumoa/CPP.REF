// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Platform;

namespace AE.SourceTree;

public record CompileTreeNode : CompileNode
{
    public required TaskCompletionSource CompileTask { get; init; }
}
