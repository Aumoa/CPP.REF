// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Xml;

namespace BuildTool.Compilers.VisualStudio;

internal static class XmlVisualStudioExtensions
{
    public static XmlElement NewElement(this XmlNode Node, string Name, string Text = "")
    {
        XmlElement NewElem;
        if (Node is XmlDocument Doc)
        {
            NewElem = Doc.CreateElement(Name);
        }
        else
        {
            NewElem = Node.OwnerDocument!.CreateElement(Name);
        }

        Node.AppendChild(NewElem);
        if (!string.IsNullOrEmpty(Text))
        {
            NewElem.InnerText = Text;
        }
        return NewElem;
    }

    public static XmlElement NewElementSingleAttr(this XmlNode Node, string Name, string AttrName, string AttrValue)
    {
        XmlElement NewElem = Node.NewElement(Name);
        NewElem.SetAttribute(AttrName, AttrValue);
        return NewElem;
    }

    public static XmlElement NewProjectConfiguration(this XmlNode Node, string Include, string InConfiguration, string InPlatform)
    {
        XmlElement ProjectConfiguration = Node.NewElement("ProjectConfiguration");
        {
            ProjectConfiguration.SetAttribute("Include", Include);
            ProjectConfiguration.NewElement("Configuration", InConfiguration);
            ProjectConfiguration.NewElement("Platform", InPlatform);
        }
        return ProjectConfiguration;
    }

    public static XmlElement NewElementPropertyGroup(this XmlNode Node, string? Condition = null, string Label = "")
    {
        XmlElement Elem = Node.NewElement("PropertyGroup");
        if (!string.IsNullOrEmpty(Condition))
        {
            Elem.SetAttribute("Condition", Condition);
        }
        if (!string.IsNullOrEmpty(Label))
        {
            Elem.SetAttribute("Label", Label);
        }
        return Elem;
    }

    public static XmlElement NewElementImport(this XmlNode Node, string Project, string Condition = "", string Label = "")
    {
        XmlElement Elem = Node.NewElement("Import");
        Elem.SetAttribute("Project", Project);
        if (!string.IsNullOrEmpty(Condition))
        {
            Elem.SetAttribute("Condition", Condition);
        }
        if (!string.IsNullOrEmpty(Label))
        {
            Elem.SetAttribute("Label", Label);
        }
        return Elem;
    }
    public static XmlElement NewElementImportGroup(this XmlNode Node, string Label, string Condition = "")
    {
        XmlElement Elem = Node.NewElement("ImportGroup");
        Elem.SetAttribute("Label", Label);
        if (!string.IsNullOrEmpty(Condition))
        {
            Elem.SetAttribute("Condition", Condition);
        }
        return Elem;
    }

    public static XmlElement NewElementItemDefinitionGroup(this XmlNode Node, string Condition)
    {
        XmlElement Elem = Node.NewElement("ItemDefinitionGroup");
        if (!string.IsNullOrEmpty(Condition))
        {
            Elem.SetAttribute("Condition", Condition);
        }
        return Elem;
    }

    public static XmlElement NewElementItemInclude(this XmlNode Node, string Type, string Path)
    {
        XmlElement Elem = Node.NewElement(Type);
        Elem.SetAttribute("Include", Path);
        return Elem;
    }
}
