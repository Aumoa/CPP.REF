// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Xml;

namespace AE.Misc;

public static class XmlExtensions
{
    public static XmlDeclaration AddXmlDeclaration(this XmlDocument Self, string Version, string? Encoding = null, string? Standalone = null)
    {
        XmlDeclaration Decl = Self.CreateXmlDeclaration(Version, Encoding, Standalone);
        Self.AppendChild(Decl);
        return Decl;
    }

    public static XmlElement AddElement(this XmlNode Self, string Name, string? NamespaceUri = null)
    {
        XmlDocument? Doc = Self as XmlDocument;
        Doc ??= Self.OwnerDocument!;

        XmlElement Elem = NamespaceUri == null ? Doc.CreateElement(Name, Self.NamespaceURI) : Doc.CreateElement(Name, NamespaceUri);
        Self.AppendChild(Elem);
        return Elem;
    }

    public static XmlElement AddPropertyGroup(this XmlNode self, string? condition = null)
    {
        var element = self.AddElement("PropertyGroup");
        if (condition != null)
        {
            element.SetAttribute("Condition", condition);
        }
        return element;
    }

    public static T Configure<T>(this T self, Action<T> configure) where T : XmlNode
    {
        configure(self);
        return self;
    }

    public static string Serialize(this XmlDocument Self, bool bIndent = false)
    {
        using var Writer = new StringWriter();
        using var Writer2 = XmlWriter.Create(Writer, new XmlWriterSettings
        {
            Indent = bIndent
        });
        Self.WriteTo(Writer2);
        Writer2.Flush();
        return Writer.GetStringBuilder().ToString();
    }
}
