// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Text;

namespace AylaEngine;

public partial class CSharpProject
{
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
