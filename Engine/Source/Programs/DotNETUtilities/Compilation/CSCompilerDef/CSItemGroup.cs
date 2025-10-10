using System.Xml;

namespace AylaEngine;

public record CSItemGroup(CSCondition? Condition, CSReference[] References, CSUsing[] Usings, CSCommonItemGroup[] Additionals) : CSElement
{
    public bool IsEmpty => References.Length == 0;

    public override string GenerateXml(string? csprojPath)
    {
        List<string> xmls = [];
        if (References.Length > 0)
        {
            string conditionAttr = Condition != null ? $" Condition=\"{Condition}\"" : string.Empty;
            string innerXml = IndentLines(string.Join("\n", References.Select(r => r.GenerateXml(csprojPath))));
            xmls.Add($"""
<ItemGroup{conditionAttr}>
{innerXml}
</ItemGroup>
""");
        }

        if (Usings.Length > 0)
        {
            string conditionAttr = Condition != null ? $" Condition=\"{Condition}\"" : string.Empty;
            string innerXml = IndentLines(string.Join("\n", Usings.Select(u => u.GenerateXml(csprojPath))));
            xmls.Add($"""
<ItemGroup{conditionAttr}>
{innerXml}
</ItemGroup>
""");
        }

        if (Additionals.Length > 0)
        {
            string conditionAttr = Condition != null ? $" Condition=\"{Condition}\"" : string.Empty;
            string innerXml = IndentLines(string.Join("\n", Additionals.Select(u => u.GenerateXml(csprojPath))));
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

        var references = new List<CSReference>();
        var usings = new List<CSUsing>();
        var additionals = new List<CSCommonItemGroup>();
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
                case "None":
                    additionals.Add(CSCommonItemGroup.Parse(item));
                    break;
                default:
                    throw new FormatException($"Unknown element '{item.Name}' in ItemGroup.");
            }
        }

        return new CSItemGroup(condition, [.. references], [.. usings], []);
    }
}
