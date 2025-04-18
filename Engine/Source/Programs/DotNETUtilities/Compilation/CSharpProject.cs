// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Text;

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

    private string GenerateInnerXml(IndentResolver indent)
    {
        return string.Join("\n\n", m_PropertyGroups.Select(p => GenerateXml(p, indent))
            .Concat(m_ItemGroups.Where(p => p.IsEmpty == false).Select(p => GenerateXml(p, indent))));
    }
}
