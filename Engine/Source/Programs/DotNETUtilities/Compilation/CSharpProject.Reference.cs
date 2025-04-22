// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Xml;

namespace AylaEngine;

public partial class CSharpProject
{
    public abstract record ReferenceBase
    {
        public required string Include { get; init; }

        public abstract string ReferencedAssemblyPath(Condition? condition, string sourceDirectory);
    }

    public record Reference : ReferenceBase
    {
        public string? HintPath { get; init; }

        public override string ToString()
        {
            if (HintPath == null)
            {
                return
                $"""
<Reference Include="{Include}" />
""";
            }
            else
            {
                return
$"""
<Reference Include="{Include}">
  <HintPath>{HintPath}</HintPath>
</Reference>
""";
            }
        }

        public override string ReferencedAssemblyPath(Condition? condition, string sourceDirectory)
        {
            return HintPath ?? throw new InvalidOperationException($"HintPath is not provided for {Include} assembly.");
        }

        public static Reference Parse(XmlElement xml)
        {
            var include = xml.GetAttributeNode("Include") ?? throw new FormatException("Missing 'Include' attribute.");
            string? hintPath = null;
            foreach (XmlElement inner in xml)
            {
                if (inner.Name == "HintPath")
                {
                    hintPath = inner.InnerText;
                }
            }

            return new Reference
            {
                Include = include.Value,
                HintPath = hintPath,
            };
        }
    }

    public record ProjectReference : ReferenceBase
    {
        public override string ToString()
        {
            return
$"""
<ProjectReference Include="{Include}" />
""";
        }

        public override string ReferencedAssemblyPath(Condition? condition, string sourceDirectory)
        {
            var depend = CSharpProject.Parse(File.ReadAllText(Include)).Freeze(condition);
            var projectName = Path.GetFileNameWithoutExtension(Include);
            return Path.Combine(depend.PropertyGroup.OutputPath ?? depend.PropertyGroup.GetOutputPath(sourceDirectory, condition?.Configuration ?? "Shipping"), projectName + ".dll");
        }

        public static ProjectReference Parse(XmlElement xml)
        {
            var include = xml.GetAttributeNode("Include") ?? throw new FormatException("Missing 'Include' attribute.");
            return new ProjectReference
            {
                Include = include.Value,
            };
        }
    }
}
