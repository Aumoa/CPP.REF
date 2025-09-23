using System.Xml;

namespace AylaEngine;

public record CSItemGroup(CSCondition? Condition, CSReferenceBase[] References, CSUsing[] Usings) : CSElement
{
    public bool IsEmpty => References.Length == 0;

    public override string GenerateXml()
    {
        List<string> xmls = [];
        if (References.Length > 0)
        {
            string conditionAttr = Condition != null ? $" Condition=\"{Condition}\"" : string.Empty;
            string innerXml = IndentLines(string.Join("\n", References.Select(r => r.GenerateXml())));
            xmls.Add($"""
<ItemGroup{conditionAttr}>
{innerXml}
</ItemGroup>
""");
        }

        if (Usings.Length > 0)
        {
            string conditionAttr = Condition != null ? $" Condition=\"{Condition}\"" : string.Empty;
            string innerXml = IndentLines(string.Join("\n", Usings.Select(u => u.GenerateXml())));
            xmls.Add($"""
<ItemGroup{conditionAttr}>
{innerXml}
</ItemGroup>
""");
        }

        return string.Join("\n\n", xmls);
    }

    public static CSItemGroup Parse(XmlElement xml)
    {
        var conditionAttr = xml.Attributes.GetNamedItem("Condition");
        CSCondition? condition = null;
        if (conditionAttr != null)
        {
            condition = CSCondition.Parse(conditionAttr.Value ?? string.Empty);
        }

        var references = new List<CSReferenceBase>();
        var usings = new List<CSUsing>();
        foreach (XmlElement item in xml)
        {
            switch (item.Name)
            {
                case "ProjectReference":
                    references.Add(CSProjectReference.Parse(item));
                    break;
                case "Using":
                    usings.Add(CSUsing.Parse(item));
                    break;
                default:
                    throw new FormatException($"Unknown element '{item.Name}' in ItemGroup.");
            }
        }

        return new CSItemGroup(condition, [.. references], [.. usings]);
    }
}
