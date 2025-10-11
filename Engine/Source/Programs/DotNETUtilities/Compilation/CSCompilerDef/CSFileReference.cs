using System.Security;

namespace AylaEngine;

public record class CSFileReference(string AssemblyName, string HintPath) : CSReference(AssemblyName)
{
    public override string GenerateXml(string? csprojPath)
    {
        return $"""
<Reference Include="{SecurityElement.Escape(Include)}">
  <HintPath>{SecurityElement.Escape(HintPath)}</HintPath>
</Reference>
""";
    }

    public override string ReferencedAssemblyPath(CSCondition? condition, Dictionary<string, CSProject> virtualProjects, string projectDirectory, HashSet<string> referencedAssemblies)
    {
        if (referencedAssemblies.Contains(AssemblyName))
        {
            return string.Empty;
        }

        if (Path.IsPathRooted(HintPath))
        {
            return HintPath;
        }
        else
        {
            return Path.Combine(projectDirectory, HintPath);
        }
    }
}
