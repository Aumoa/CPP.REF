// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Text;
using System.Xml;

namespace AylaEngine;

public partial class CSharpProject
{
    public record AssemblyAttribute
    {
        public required Version DotNETVersion { get; init; }

        public string GenerateScript()
        {
            return
$"""
[assembly: global::System.Runtime.Versioning.TargetFramework(".NETCoreApp,Version=v9.0", FrameworkDisplayName = ".NET {DotNETVersion.ToString(2)}")]
""";
        }
    }

    public record AssemblyInfo
    {
        public string? Company { get; init; }
        public string? Configuration { get; init; }
        public string? Product { get; init; }
        public string? Title { get; init; }
        public Version? Version { get; init; }

        public string GenerateScript()
        {
            List<string> assemblyAttributes = [];
            
            if (Company != null)
            {
                assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyCompany", Company));
            }
            if (Configuration != null)
            {
                assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyConfiguration", Configuration));
            }
            if (Product != null)
            {
                assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyProduct", Product));
            }
            if (Title != null)
            {
                assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyTitle", Title));
            }
            if (Version != null)
            {
                assemblyAttributes.Add(FormatAssemblyAttribute("AssemblyVersion", Version.ToString(4)));
            }

            return
$"""
{string.Join('\n', assemblyAttributes)}
""";

            string FormatAssemblyAttribute(string name, string value)
            {
                return $"[assembly: global::System.Reflection.{name}(\"{value}\")]";
            }
        }
    }

    private readonly IEnumerable<PropertyGroup> m_PropertyGroups;
    private readonly IEnumerable<ItemGroup> m_ItemGroups;

    public CSharpProject(IEnumerable<PropertyGroup> propertyGroups, IEnumerable<ItemGroup> itemGroups)
    {
        m_PropertyGroups = propertyGroups;
        m_ItemGroups = itemGroups;
    }

    public string GenerateXml()
    {
        var resolver = new IndentResolver();

        return
$"""
<Project Sdk="Microsoft.NET.Sdk">
{resolver.Indent(GenerateInnerXml)}
</Project>

""".Replace("\r\n", "\n");
    }

    public FrozenCSharpProject Freeze(Condition? condition)
    {
        PropertyGroup propertyGroup = new PropertyGroup();
        ItemGroup itemGroup = new ItemGroup();

        foreach (var item in m_PropertyGroups)
        {
            if (item.Condition == null || (condition == null || item.Condition.Contains(condition)))
            {
                propertyGroup = item with
                {
                    TargetFramework = item.TargetFramework != null ? item.TargetFramework : propertyGroup.TargetFramework,
                    Nullable = item.Nullable != null ? item.Nullable : propertyGroup.Nullable,
                    RootNamespace = item.RootNamespace != null ? item.RootNamespace : propertyGroup.RootNamespace,
                    Platforms = item.Platforms != null ? item.Platforms : propertyGroup.Platforms,
                    Configurations = item.Configurations != null ? item.Configurations : propertyGroup.Configurations,
                    AppendTargetFrameworkToOutputPath = item.AppendTargetFrameworkToOutputPath != null ? item.AppendTargetFrameworkToOutputPath : propertyGroup.AppendTargetFrameworkToOutputPath,
                    PropertyTarget = item.PropertyTarget != null ? item.PropertyTarget : propertyGroup.PropertyTarget,
                    Optimize = item.Optimize != null ? item.Optimize : propertyGroup.Optimize,
                    OutputPath = item.OutputPath != null ? item.OutputPath : propertyGroup.OutputPath
                };
            }
        }

        foreach (var item in m_ItemGroups)
        {
            if (item.Condition == null || (condition == null || item.Condition.Contains(condition)))
            {
                itemGroup = item with
                {
                    References = itemGroup.References != null ? itemGroup.References.Concat(item.References ?? []) : item.References
                };
            }
        }

        itemGroup = itemGroup with
        {
            References = itemGroup.References?.Distinct()
        };

        return new FrozenCSharpProject { PropertyGroup = propertyGroup, ItemGroup = itemGroup };
    }

    private string GenerateInnerXml(IndentResolver indent)
    {
        return string.Join("\n\n", m_PropertyGroups.Select(p => GenerateXml(p, indent))
            .Concat(m_ItemGroups.Where(p => p.IsEmpty == false).Select(p => GenerateXml(p, indent))));
    }

    public static CSCompiler.SourceCodeProvider GenerateAssemblyAttribute(Version dotNETVersion)
    {
        var assemblyAttribute = new AssemblyAttribute { DotNETVersion = dotNETVersion };
        var script = assemblyAttribute.GenerateScript();
        return CSCompiler.SourceCodeProvider.FromSourceCode(".AssemblyAttribute.cs", script);
    }

    public static CSCompiler.SourceCodeProvider GenerateAssemblyInfo(string? company, string? configuration, string? product, string? title, Version? version)
    {
        var assemblyInfo = new AssemblyInfo
        {
            Company = company,
            Configuration = configuration,
            Product = product,
            Title = title,
            Version = version
        };
        var script = assemblyInfo.GenerateScript();
        return CSCompiler.SourceCodeProvider.FromSourceCode(".AssemblyInfo.cs", script);
    }

    public static CSharpProject Parse(string project)
    {
        XmlDocument document = new();
        document.LoadXml(project);

        foreach (XmlElement item in document)
        {
            if (item.Name == "Project")
            {
                var sdk = (XmlAttribute?)item.Attributes?.GetNamedItem("Sdk");
                if (sdk == null)
                {
                    throw new FormatException("Invalid project file format.");
                }

                if (sdk.Value != "Microsoft.NET.Sdk")
                {
                    throw new FormatException("Invalid project file format.");
                }

                return ParseProject(item);
            }
        }

        throw new FormatException("Invalid project file format.");

        CSharpProject ParseProject(XmlElement project)
        {
            List<PropertyGroup> propertyGroups = [];
            List<ItemGroup> itemGroups = [];

            foreach (XmlElement child in project)
            {
                switch (child.Name)
                {
                    case "PropertyGroup":
                        propertyGroups.Add(PropertyGroup.Parse(child));
                        break;
                    case "ItemGroup":
                        itemGroups.Add(ItemGroup.Parse(child));
                        break;
                    default:
                        throw new FormatException($"Unknown element '{child.Name}' in project file.");
                }
            }

            return new CSharpProject(propertyGroups, itemGroups);
        }
    }
}
