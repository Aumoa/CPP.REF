using System.Security;
using System.Xml;

namespace AylaEngine;

public record class CSProjectReference(string Include, bool? Private) : CSReferenceBase(Include)
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

    public override string ReferencedAssemblyPath(CSCondition? condition, string configuration, string projectDirectory)
    {
        XmlDocument doc = new();
        doc.LoadXml(File.ReadAllText(Include));
        var includeProject = doc.ChildNodes.OfType<XmlElement>().First();
        var depend = CSProject.Parse(includeProject).Freeze(condition);
        var projectName = Path.GetFileNameWithoutExtension(Include);
        var dependOutputPath = depend.PropertyGroup.OutputPath ?? Path.Combine(projectDirectory, "bin", configuration, depend.PropertyGroup.TargetFramework?.ToFrameworkString() ?? "net9.0");
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
