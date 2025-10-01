using System.Reflection;
using System.Security;
using System.Xml;

namespace AylaEngine;

public record class CSProject(string Sdk, CSPropertyGroup[] PropertyGroups, CSItemGroup[] ItemGroups, CSCondition? Condition) : CSElement
{
    public override string GenerateXml(string? csprojPath)
    {
        var children = PropertyGroups.Cast<CSElement>().Concat(ItemGroups.Cast<CSElement>()).ToArray();
        var childrenXml = IndentLines(string.Join("\n\n", children.Select(child => child.GenerateXml(csprojPath))));
        return $"""
<Project Sdk="{SecurityElement.Escape(Sdk)}">
            
{childrenXml}

</Project>
""";
    }

    public static CSProject Parse(XmlElement project)
    {
        var sdk = project.GetAttribute("Sdk");
        if (string.IsNullOrEmpty(sdk))
        {
            throw new FormatException("Missing 'Sdk' attribute in Project element.");
        }

        List<CSPropertyGroup> propertyGroups = [];
        List<CSItemGroup> itemGroups = [];
        foreach (var childNode in project.ChildNodes.OfType<XmlElement>())
        {
            switch (childNode.Name)
            {
                case "PropertyGroup":
                    propertyGroups.Add(CSPropertyGroup.Parse(childNode));
                    break;
                case "ItemGroup":
                    itemGroups.Add(CSItemGroup.Parse(childNode));
                    break;
                default:
                    throw new FormatException($"Unknown element '{childNode.Name}' in Project.");
            }
        }

        return new CSProject(sdk, propertyGroups.ToArray(), itemGroups.ToArray(), null);
    }

    public CSProject Freeze(CSCondition? condition)
    {
        var frozenPropertyGroups = PropertyGroups
            .Where(pg => pg.Condition == null || condition == null || pg.Condition.Contains(condition))
            .ToArray();
        var frozenItemGroups = ItemGroups
            .Where(ig => ig.Condition == null || condition == null || ig.Condition.Contains(condition))
            .ToArray();
        return this with { PropertyGroups = frozenPropertyGroups, ItemGroups = frozenItemGroups, Condition = condition };
    }

    public CSPropertyGroup PropertyGroup
    {
        get
        {
            CSPropertyGroup builtPropertyGroup = new CSPropertyGroup(null, null, null, null, null, null, null, null, null, null, null, null, [], [], null, null, [], null);
            foreach (var pg in PropertyGroups)
            {
                builtPropertyGroup = builtPropertyGroup with
                {
                    OutputPath = pg.OutputPath ?? builtPropertyGroup.OutputPath,
                    TargetFramework = pg.TargetFramework ?? builtPropertyGroup.TargetFramework,
                    ImplicitUsings = pg.ImplicitUsings ?? builtPropertyGroup.ImplicitUsings,
                    Nullable = pg.Nullable ?? builtPropertyGroup.Nullable,
                    AssemblyName = pg.AssemblyName ?? builtPropertyGroup.AssemblyName,
                    RootNamespace = pg.RootNamespace ?? builtPropertyGroup.RootNamespace,
                    AllowUnsafeBlocks = pg.AllowUnsafeBlocks ?? builtPropertyGroup.AllowUnsafeBlocks,
                    EnableRuntimeMarshalling = pg.EnableRuntimeMarshalling ?? builtPropertyGroup.EnableRuntimeMarshalling,
                    EnableDefaultNamespace = pg.EnableDefaultNamespace ?? builtPropertyGroup.EnableDefaultNamespace,
                    AppendTargetFrameworkToOutputPath = pg.AppendTargetFrameworkToOutputPath ?? builtPropertyGroup.AppendTargetFrameworkToOutputPath,
                    AppendRuntimeIdentifierToOutputPath = pg.AppendRuntimeIdentifierToOutputPath ?? builtPropertyGroup.AppendRuntimeIdentifierToOutputPath,
                    Configurations = builtPropertyGroup.Configurations.Concat(pg.Configurations).Distinct().ToArray(),
                    Platforms = builtPropertyGroup.Platforms.Concat(pg.Platforms).Distinct().ToArray(),
                    OutputType = pg.OutputType ?? builtPropertyGroup.OutputType,
                    Optimize = pg.Optimize ?? builtPropertyGroup.Optimize,
                    DefineConstants = builtPropertyGroup.DefineConstants.Concat(pg.DefineConstants).Distinct().ToArray(),
                    PlatformTarget = pg.PlatformTarget ?? builtPropertyGroup.PlatformTarget
                };
            }

            return builtPropertyGroup;
        }
    }

    public CSItemGroup ItemGroup
    {
        get
        {
            CSItemGroup builtItemGroup = new CSItemGroup(null, [], [], []);
            foreach (var ig in ItemGroups)
            {
                builtItemGroup = builtItemGroup with
                {
                    References = builtItemGroup.References.Concat(ig.References).Distinct().ToArray(),
                    Usings = builtItemGroup.Usings.Concat(ig.Usings).Distinct().ToArray()
                };
            }
            return builtItemGroup;
        }
    }

    public CSSourceCode GenerateGlobals()
    {
        List<string> usings = [];

        if (PropertyGroup.ImplicitUsings == true)
        {
            usings.Add("global::System");
            usings.Add("global::System.Collections.Generic");
            usings.Add("global::System.IO");
            usings.Add("global::System.Linq");
            usings.Add("global::System.Net.Http");
            usings.Add("global::System.Threading");
            usings.Add("global::System.Threading.Tasks");
        }

        foreach (var u in ItemGroup.Usings)
        {
            if (string.IsNullOrEmpty(u.Alias))
            {
                usings.Add($"global::{u.Name}");
            }
            else
            {
                usings.Add($"{u.Alias} = global::{u.Name}");
            }
        }

        string sourceCodeText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

{string.Join('\n', usings.Select(FormatUsing))}

""";

        return CSSourceCode.FromString(".g.cs", sourceCodeText);

        string FormatUsing(string u)
        {
            return $"global using {u};";
        }
    }

    public CSSourceCode GenerateAssemblyAttribute(string? company, string? configuration, string? product, string? title, Version? version)
    {
        string frameworkAssemblyQualifiedName;
        string frameworkDisplayName;
        var targetFramework = (PropertyGroup.TargetFramework ?? CSTargetFramework.Net0900);
        
        switch (Sdk)
        {
            case "Microsoft.NET.Sdk":
                frameworkAssemblyQualifiedName = targetFramework.ToFrameworkAssemblyQualifiedName();
                frameworkDisplayName = targetFramework.ToFrameworkDisplayName();
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(Sdk), Sdk, null);
        }

        List<string> assemblyAttributes = [];
        if (company != null)
        {
            assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyCompany", company));
        }
        if (configuration != null)
        {
            assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyConfiguration", configuration));
        }
        if (product != null)
        {
            assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyProduct", product));
        }
        if (title != null)
        {
            assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyTitle", title));
        }
        if (version != null)
        {
            assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyVersion", version.ToString()));
        }

        string sourceCodeText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

[assembly: global::System.Runtime.Versioning.TargetFramework("{frameworkAssemblyQualifiedName}", FrameworkDisplayName = "{frameworkDisplayName}")]
{string.Join('\n', assemblyAttributes)}

""";

        return CSSourceCode.FromString(".AssemblyAttributes.cs", sourceCodeText);

        string FormatAssemblyAttribute(string name, string value)
        {
            return $"[assembly: global::System.Reflection.{name}(\"{value}\")]";
        }
    }
}
