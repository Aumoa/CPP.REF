using System.Security;
using System.Xml;

namespace AylaEngine;

public record class CSProjectReference(string Include, bool? Private) : CSReference(Include)
{
    public override string GenerateXml()
    {
        if (Private.HasValue)
        {
            return $"""
<ProjectReference Include="{SecurityElement.Escape(Include)}">
  <Private>{Private.Value.ToString().ToLower()}</Private>
</ProjectReference>
""";
        }
        else
        {
            return $"""
<ProjectReference Include="{SecurityElement.Escape(Include)}" />
""";
        }
    }

    public override string ReferencedAssemblyPath(CSCondition? condition, string projectDirectory, HashSet<string> referencedAssemblies)
    {
        XmlDocument doc = new();
        doc.LoadXml(File.ReadAllText(Include));
        var includeProject = doc.ChildNodes.OfType<XmlElement>().First();
        var depend = CSProject.Parse(includeProject).Freeze(condition);
        string dependProjectPath;
        if (Path.IsPathRooted(Include))
        {
            dependProjectPath = Include;
        }
        else
        {
            dependProjectPath = Path.Combine(projectDirectory, Include);
        }

        string dependProjectFolder = Path.GetDirectoryName(dependProjectPath)!;
        string dependOutputPath = depend.PropertyGroup.ParseOutputPath(dependProjectFolder);
        var projectName = Path.GetFileNameWithoutExtension(dependProjectPath);
        return Path.Combine(dependOutputPath, projectName + ".dll");
    }

    public static CSProjectReference Parse(XmlElement xml)
    {
        var include = xml.GetAttributeNode("Include") ?? throw new FormatException("Missing 'Include' attribute.");
        bool? @private = null;
        foreach (XmlElement inner in xml)
        {
            switch (inner.Name)
            {
                case "Private":
                    @private = bool.Parse(inner.InnerText);
                    break;

            }
        }

        return new CSProjectReference(include.Value, @private);
    }
}
