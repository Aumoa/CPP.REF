using System.Security;
using System.Xml;

namespace AylaEngine;

public record CSUsing(string Name, string Alias) : CSElement
{
    public override string GenerateXml()
    {
        return $"""
<Using Include="{SecurityElement.Escape(Name)}">
  <Alias>{SecurityElement.Escape(Alias)}</Alias>
</Using>
""";
    }

    public static CSUsing Parse(XmlElement xml)
    {
        var include = xml.GetAttributeNode("Include") ?? throw new FormatException("Missing 'Include' attribute.");
        var aliasAttr = xml.GetAttributeNode("Alias");
        var alias = aliasAttr?.Value ?? throw new FormatException("Missing 'Alias' attribute.");
        return new CSUsing(include.Value, alias);
    }
}
