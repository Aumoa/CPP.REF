// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Xml;

namespace AylaEngine;

public partial class CSharpProject
{
    public record ReferenceBase
    {
        public required string Include { get; init; }
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

        public static Reference Parse(XmlElement xml)
        {
            var include = xml.GetAttributeNode("Include") ?? throw new FormatException("Missing 'Include' attribute.");
            string? hintPath = null;
            foreach (XmlElement inner in xml)
            {
                if (inner.Name == "HintPath")
                {
                    hintPath = inner.Value;
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
