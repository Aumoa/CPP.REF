// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Text.Json.Nodes;
using System.Xml.Linq;

using AE.Exceptions;

using static AE.Projects.HeaderCompileItem;

namespace AE.Projects;

public class ModuleCompileItem : CompileItem
{
    [SetsRequiredMembers]
    public ModuleCompileItem(string SourceCode, JsonNode? Node, ModuleDependenciesResolver.ModuleDependencyCache DependencyCache)
    {
        this.SourceCode = SourceCode;
        this.DependencyCache = DependencyCache;

        if (Node == null)
        {
            throw new TerminateException(10, CoreStrings.Errors.InvalidModuleFormat, SourceCode);
        }

        if (Node["rules"] is not JsonArray Rules)
        {
            throw new TerminateException(10, CoreStrings.Errors.InvalidModuleFormat, SourceCode);
        }

        Debug.Assert(Rules.Count == 1);
        if (Rules[0] is not JsonNode Rule)
        {
            throw new TerminateException(10, CoreStrings.Errors.InvalidModuleFormat, SourceCode);
        }

        Output = (string?)Rule["primary-output"];
        if (Output == null)
        {
            throw new TerminateException(10, CoreStrings.Errors.InvalidModuleFormat, SourceCode);
        }

        if (Rule["outputs"] is not JsonArray Outputs)
        {
            throw new TerminateException(10, CoreStrings.Errors.InvalidModuleFormat, SourceCode);
        }

        this.InterfaceOutput = null;
        foreach (var Output in Outputs)
        {
            string? OutputStr = (string?)Output;
            Debug.Assert(OutputStr != null);
            if (OutputStr == "-")
            {
                continue;
            }

            Debug.Assert(InterfaceOutput == null);
            this.InterfaceOutput = Path.Combine(Path.GetDirectoryName(this.Output)!, OutputStr);
        }

        if (Rule["provides"] is JsonArray Provides)
        {
            foreach (var Provide in Provides)
            {
                string? LogicalName = (string?)Provide?["logical-name"];
                if (LogicalName == null)
                {
                    continue;
                }

                this.LogicalName = LogicalName;
                Debug.Assert((bool?)Provide!["is-interface"] == true);
            }
        }
        
        if (string.IsNullOrEmpty(LogicalName))
        {
            LogicalName = "private-" + Path.GetFileNameWithoutExtension(SourceCode);
        }

        JsonArray? Requires = Rule["requires"] as JsonArray;
        Debug.Assert(Requires != null);

        List<DependencyItem> Dependencies = new();
        foreach (var Require in Requires)
        {
            Debug.Assert(Require != null);
            string LogicalName = (string)Require["logical-name"]!;
            string? SourcePath = (string?)Require["source-path"];
            string? LookupMethodStr = (string?)Require["lookup-method"];

            if (SourcePath != null && LookupMethodStr != null)
            {
                LookupMethod Method = LookupMethodStr switch
                {
                    "include-angle" => LookupMethod.Angle,
                    "include-quote" => LookupMethod.Quote,
                    _ => throw new InvalidOperationException("Internal error.")
                };
                Dependencies.Add(new HeaderDependencyItem(SourcePath, Method, LogicalName));
            }
            else
            {
                Dependencies.Add(new ModuleDependencyItem(LogicalName));
            }
        }
        this.Dependencies = Dependencies.ToArray();
    }

    public required ModuleDependenciesResolver.ModuleDependencyCache DependencyCache { get; init; }
}
