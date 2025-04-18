// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Xml;
using Microsoft.CodeAnalysis;

namespace AylaEngine;

public partial class CSharpProject
{
    public record PropertyGroup
    {
        public Condition? Condition { get; init; }

        public string? TargetFramework { get; init; }

        public NullableContextOptions? Nullable { get; init; }

        public string? RootNamespace { get; init; }

        public IEnumerable<string>? Platforms { get; init; }

        public IEnumerable<string>? Configurations { get; init; }

        public bool? AppendTargetFrameworkToOutputPath { get; init; }

        public string? PropertyTarget { get; init; }

        public bool? Optimize { get; init; }

        public string? OutputPath { get; init; }

        public static PropertyGroup Parse(XmlElement xml)
        {
            var conditionAttr = xml.Attributes.GetNamedItem("Condition");
            Condition? condition = null;
            if (conditionAttr != null)
            {
                condition = Condition.Parse(conditionAttr.Value ?? string.Empty);
            }

            string? targetFramework = null;
            NullableContextOptions? nullable = null;
            string? rootNamespace = null;
            string? platforms = null;
            string? configurations = null;
            bool? appendTargetFrameworkToOutputPath = null;
            string? propertyTarget = null;
            bool? optimize = null;
            string? outputPath = null;

            foreach (XmlElement item in xml)
            {
                switch (item.Name)
                {
                    case "TargetFramework":
                        targetFramework = item.InnerText;
                        break;
                    case "Nullable":
                        nullable = Enum.Parse<NullableContextOptions>(item.InnerText, true);
                        break;
                    case "RootNamespace":
                        rootNamespace = item.InnerText;
                        break;
                    case "Platforms":
                        platforms = item.InnerText;
                        break;
                    case "Configurations":
                        configurations = item.InnerText;
                        break;
                    case "AppendTargetFrameworkToOutputPath":
                        appendTargetFrameworkToOutputPath = bool.Parse(item.InnerText);
                        break;
                    case "PropertyTarget":
                        propertyTarget = item.InnerText;
                        break;
                    case "Optimize":
                        optimize = bool.Parse(item.InnerText);
                        break;
                    case "OutputPath":
                        outputPath = item.InnerText;
                        break;
                }
            }

            return new PropertyGroup
            {
                Condition = condition,
                TargetFramework = targetFramework,
                Nullable = nullable,
                RootNamespace = rootNamespace,
                Platforms = platforms?.Split(';'),
                Configurations = configurations?.Split(';'),
                AppendTargetFrameworkToOutputPath = appendTargetFrameworkToOutputPath,
                PropertyTarget = propertyTarget,
                Optimize = optimize,
                OutputPath = outputPath
            };
        }
    }

    private string GenerateXml(PropertyGroup value, IndentResolver indent)
    {
        string? conditionString = null;
        if (value.Condition != null)
        {
            conditionString = $" Condition=\"{value.Condition.ToString()}\"";
        }

        return $"""
<PropertyGroup{conditionString}>
{indent.Indent(GenerateInnerXml)}
</PropertyGroup>
""";

        string GenerateInnerXml(IndentResolver indent)
        {
            List<(string, string)> propertyName = [];

            if (value.TargetFramework != null)
            {
                propertyName.Add(("TargetFramework", value.TargetFramework));
            }
            if (value.Nullable != null)
            {
                propertyName.Add(("Nullable", value.Nullable.Value.ToString().ToLower()));
            }
            if (value.RootNamespace != null)
            {
                propertyName.Add(("RootNamespace", value.RootNamespace));
            }
            if (value.Platforms != null)
            {
                propertyName.Add(("Platforms", string.Join(';', value.Platforms)));
            }
            if (value.Configurations != null)
            {
                propertyName.Add(("Configurations", string.Join(';', value.Configurations)));
            }
            if (value.AppendTargetFrameworkToOutputPath != null)
            {
                propertyName.Add(("AppendTargetFrameworkToOutputPath", value.AppendTargetFrameworkToOutputPath.Value.ToString()));
            }
            if (value.PropertyTarget != null)
            {
                propertyName.Add(("PropertyTarget", value.PropertyTarget));
            }
            if (value.Optimize != null)
            {
                propertyName.Add(("Optimize", value.Optimize.Value.ToString()));
            }
            if (value.OutputPath != null)
            {
                propertyName.Add(("OutputPath", value.OutputPath));
            }

            return string.Join('\n', propertyName.Select(p => $"<{p.Item1}>{p.Item2}</{p.Item1}>"));
        }
    }
}
