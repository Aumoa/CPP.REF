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
    public HeaderCompileItem(ProjectDirectory StdDir, string Name, string SourceCode, string LookupMethod) : base(SourceCode)
    {
        this.Name = Name;
        if (LookupMethod == "include-angle")
        {
            Method = HeaderCompileItem.LookupMethod.Angle;
        }
        else
        {
            Method = HeaderCompileItem.LookupMethod.Quote;
        }

        string StdOutPath = StdDir.Intermediate.StdBuild;
        InterfaceOutput = Path.Combine(StdOutPath, Path.ChangeExtension(Name, ".ifc"));
        Output = Path.Combine(StdOutPath, Path.ChangeExtension(Name, ".obj"));
    }

    public required string Name { get; init; }

    public required LookupMethod Method { get; init; }

    public required string InterfaceOutput { get; init; }
}
