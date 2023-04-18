// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics.CodeAnalysis;

namespace AE.Projects;

public abstract class CompileItem
{
    public CompileItem()
    {
    }

    public abstract class DependencyItem
    {
        public required string LogicalName { get; init; }
    }

    public class HeaderDependencyItem : DependencyItem
    {
        [SetsRequiredMembers]
        public HeaderDependencyItem(string SourceCode, HeaderCompileItem.LookupMethod LookupMethod, string LogicalName)
        {
            this.LogicalName = LogicalName;
            this.LookupMethod = LookupMethod;
            this.SourceCode = SourceCode;
        }

        public required HeaderCompileItem.LookupMethod LookupMethod { get; init; }

        public required string SourceCode { get; init; }
    }

    public class ModuleDependencyItem : DependencyItem
    {
        [SetsRequiredMembers]
        public ModuleDependencyItem(string LogicalName)
        {
            this.LogicalName = LogicalName;
        }
    }

    public required string SourceCode { get; init; }

    public required string? Output { get; init; }

    public required string LogicalName { get; init; }

    public required string? InterfaceOutput { get; init; }

    public required DependencyItem[] Dependencies { get; init; }
}
