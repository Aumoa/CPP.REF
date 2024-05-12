// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Xml;

namespace AylaEngine;

internal static class VisualStudioXml
{
    public static XmlElement AddPropertyGroup(this XmlNode self, string? condition = null)
    {
        var element = self.AddElement("PropertyGroup");
        if (condition != null)
        {
            element.SetAttribute("Condition", condition);
        }
        return element;
    }
}
