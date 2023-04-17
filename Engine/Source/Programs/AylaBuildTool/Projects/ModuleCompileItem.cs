// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Text.Json.Nodes;

namespace AE.Projects;

public class ModuleCompileItem : CompileItem
{
    public required ACXXModule SourceModule { get; init; }

    public required CompilerOptions Options { get; init; }

    private JsonNode? DependencyNode { get; init; }

    public struct CompilerOptions
    {
        public string Includes { get; set; }

        public string Defines { get; set; }
    }

    public record RequiredModule
    {
        public required string Name { get; init; }

        public required string SourcePath { get; init; }
    
        public required string LookupMethod { get; init; }
    }

    [SetsRequiredMembers]
    public ModuleCompileItem(ACXXModule SourceModule, CompilerOptions Options, string SourceCode, CompileTree Tree, JsonNode? Node) : base(SourceCode)
    {
        this.SourceModule = SourceModule;
        this.Options = Options;

        Debug.Assert(Node != null);
        DependencyNode = Node;
        this.SourceCode = SourceCode;

        JsonArray? Rules = Node["rules"] as JsonArray;
        Debug.Assert(Rules != null);
        JsonNode? Rule = Rules[0] as JsonNode;
        Debug.Assert(Rule != null);
        Debug.Assert(Rules.Count == 1);

        Output = (string)Rule["primary-output"]!;
        Debug.Assert(Output != null);

        JsonArray? Outputs = Rule["outputs"] as JsonArray;
        Debug.Assert(Outputs != null);
        foreach (var Output in Outputs)
        {
            string? OutputStr = (string?)Output;
            Debug.Assert(OutputStr != null);
            if (OutputStr == "-")
            {
                continue;
            }

            Debug.Assert(InterfaceOutput == null);
            InterfaceOutput = Path.Combine(Path.GetDirectoryName(this.Output)!, OutputStr);
        }

        JsonArray? Provides = Rule["provides"] as JsonArray;
        if (Provides != null)
        {
            foreach (var Provide in Provides)
            {
                string? LogicalName = (string?)Provide?["logical-name"];
                if (LogicalName == null)
                {
                    continue;
                }

                InterfaceName = LogicalName;
                Debug.Assert((bool?)Provide!["is-interface"] == true);
            }
        }

        JsonArray? Requires = Rule["requires"] as JsonArray;
        Debug.Assert(Requires != null);

        List<RequiredModule> RequiredModules = new();
        foreach (var Require in Requires)
        {
            Debug.Assert(Require != null);
            RequiredModules.Add(new RequiredModule()
            {
                Name = (string)Require["logical-name"]!,
                SourcePath = (string?)Require["source-path"] ?? string.Empty,
                LookupMethod = (string?)Require["lookup-method"] ?? string.Empty
            });
        }
        this.RequiredModules = RequiredModules.ToArray();
    }

    public required string? InterfaceOutput { get; init; }

    public required string? InterfaceName { get; init; }

    public required RequiredModule[] RequiredModules { get; init; }
}
