using System.Security;
using System.Xml;

namespace AylaEngine;

public record class CSProject(string Sdk, CSPropertyGroup[] PropertyGroups, CSItemGroup[] ItemGroups, CSCondition? Condition) : CSElement
{
    public override string GenerateXml()
    {
        var children = PropertyGroups.Cast<CSElement>().Concat(ItemGroups.Cast<CSElement>()).ToArray();
        var childrenXml = IndentLines(string.Join("\n\n", children.Select(child => child.GenerateXml())));
        return $"""
<Project Sdk=\"{SecurityElement.Escape(Sdk)}\">
            
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
            CSItemGroup builtItemGroup = new CSItemGroup(null, [], []);
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
}
