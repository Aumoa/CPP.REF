// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using DotNETUtilities;

using System.Text;
using System.Xml;

namespace BuildTool;

static class VisualStudioXmlFunctions
{
    public static XmlDeclaration CreateXmlDeclaration(this XmlDocument Doc)
    {
        XmlDeclaration Decl = Doc.CreateXmlDeclaration("1.0", "UTF-8", null);
        Doc.AppendChild(Decl);
        return Decl;
    }

    public static XmlElement NewElement(this XmlNode Node, string Name, string Text = "")
    {
        XmlElement NewElem = null;
        if (Node is XmlDocument Doc)
        {
            NewElem = Doc.CreateElement(Name);
        }
        else
        {
            NewElem = Node.OwnerDocument.CreateElement(Name);
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

    public static XmlElement NewElementPropertyGroup(this XmlNode Node, string Condition, string Label = "")
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

    public static string GetTypeString(this ModuleType MType)
    {
        switch (MType)
        {
            case ModuleType.ConsoleApplication:
            case ModuleType.Application:
                return "Application";
            case ModuleType.GameModule:
            case ModuleType.ConsoleModule:
                return "DynamicLibrary";
            case ModuleType.Vcpkg:
            default:
                return "Utility";
        }
    }

    public static string GetPlatformToolsets(this VisualStudioVersion Version)
    {
        return Version switch
        {
            VisualStudioVersion.VS2019 => "v142",
            VisualStudioVersion.VS2022 => "v143",
            _ => throw new ArgumentException(Version.ToString())
        };
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

    public static string GetSubsystemString(this ModuleType Type)
    {
        return Type switch
        {
            ModuleType.ConsoleApplication => "Console",
            _ => "Windows"
        };
    }

    public static XmlElement NewElementItemInclude(this XmlNode Node, string Type, string Path)
    {
        XmlElement Elem = Node.NewElement(Type);
        Elem.SetAttribute("Include", Path);
        return Elem;
    }

    public static void SaveIfChanged(this XmlDocument Doc, FileReference FileRef)
    {
        StringBuilder SBuilder = new();
        StringWriter SWriter = new(SBuilder);
        Doc.Save(SWriter);

        string CurrentFile = SWriter.ToString();

        if (!FileRef.IsExist)
        {
            File.WriteAllText(FileRef.FullPath, CurrentFile);
            return;
        }

        string PreviousFile = FileRef.ReadAllText();
        if (PreviousFile != CurrentFile)
        {
            File.WriteAllText(FileRef.FullPath, CurrentFile);
        }
    }
}