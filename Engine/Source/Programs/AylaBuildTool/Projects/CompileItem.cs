// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

namespace AE.Projects;

public abstract class CompileItem
{
    [SetsRequiredMembers]
    public CompileItem(string SourceCode)
    {
        this.SourceCode = SourceCode;
    }

    public required string SourceCode { get; init; }

    public string? Output { get; protected set; }
}
