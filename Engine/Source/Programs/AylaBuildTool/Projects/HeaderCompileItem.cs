// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Source;

using System.Diagnostics.CodeAnalysis;

namespace AE.Projects;

public class HeaderCompileItem : CompileItem
{
    public enum LookupMethod
    {
        Angle,
        Quote
    }

    [SetsRequiredMembers]
    public HeaderCompileItem(string Name, string SourceCode, LookupMethod Method)
    {
        this.SourceCode = SourceCode;
        this.LogicalName = Name;
        this.Dependencies = Array.Empty<DependencyItem>();
        this.Method = Method;

        string StdOutPath = Global.EngineDirectory.Intermediate.StdBuild;
        this.InterfaceOutput = Path.Combine(StdOutPath, Path.ChangeExtension(Name, ".ifc"));
        this.Output = Path.Combine(StdOutPath, Path.ChangeExtension(Name, ".obj"));
    }

    public required LookupMethod Method { get; init; }
}
