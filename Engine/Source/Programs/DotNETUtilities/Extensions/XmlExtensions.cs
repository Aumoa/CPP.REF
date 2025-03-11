// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Xml;

namespace AylaEngine;

public static class XmlExtensions
{
    public static XmlDeclaration AddXmlDeclaration(this XmlDocument self, string version, string? encoding = null, string? standalone = null)
    {
        XmlDeclaration decl = self.CreateXmlDeclaration(version, encoding, standalone);
        self.AppendChild(decl);
        return decl;
    }

    public static XmlElement AddElement(this XmlNode self, string name, string? namespaceUri = null)
    {
        var doc = self as XmlDocument;
        doc ??= self.OwnerDocument!;

        XmlElement element = namespaceUri == null ? doc.CreateElement(name, self.NamespaceURI) : doc.CreateElement(name, namespaceUri);
        self.AppendChild(element);
        return element;
    }

    public static T Configure<T>(this T self, Action<T> configure) where T : XmlNode
    {
        configure(self);
        return self;
    }
}
