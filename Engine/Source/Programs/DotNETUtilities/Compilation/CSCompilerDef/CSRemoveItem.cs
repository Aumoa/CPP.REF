using System.Security;

namespace AylaEngine;

public record CSRemoveItem(string Pattern) : CSCommonItemGroup
{
    public override string GenerateXml(string? csprojPath)
    {
        return $"""
<None Remove="{SecurityElement.Escape(Pattern)}" />
""";
    }
}
