// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Xml;

namespace AylaEngine;

public partial class CSharpProject
{

    public record ItemGroup
    {
        public Condition? Condition { get; init; }
        
        public IEnumerable<ReferenceBase>? References { get; init; }

        public bool IsEmpty => References == null || References.Any() == false;

        public static ItemGroup Parse(XmlElement xml)
        {
            var conditionAttr = xml.Attributes.GetNamedItem("Condition");
            Condition? condition = null;
            if (conditionAttr != null)
            {
                condition = Condition.Parse(conditionAttr.Value ?? string.Empty);
            }

            var references = new List<ReferenceBase>();
            foreach (XmlElement item in xml)
            {
                switch (item.Name)
                {
                    case "Reference":
                        references.Add(Reference.Parse(item));
                        break;
                    case "ProjectReference":
                        references.Add(ProjectReference.Parse(item));
                        break;
                    default:
                        throw new FormatException($"Unknown element '{item.Name}' in ItemGroup.");
                }
            }

            return new ItemGroup
            {
                Condition = condition,
                References = references,
            };
        }
    }

    private string GenerateXml(ItemGroup value, IndentResolver indent)
    {
        string? conditionString = null;
        if (value.Condition != null)
        {
            conditionString = $" Condition=\"{value.Condition.ToString()}\"";
        }

        return
$"""
<ItemGroup{conditionString}>
  {GenerateInnerXml(indent)}
</ItemGroup>
""";

        string GenerateInnerXml(IndentResolver indent)
        {
            return string.Join('\n', [..(value.References ?? Array.Empty<ReferenceBase>())]);
        }
    }
}
