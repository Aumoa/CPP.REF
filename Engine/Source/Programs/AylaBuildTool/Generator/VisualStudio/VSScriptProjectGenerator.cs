namespace AylaEngine;

internal static class VSScriptProjectGenerator
{
    public static async ValueTask GenerateAsync(ModuleProject project, CSProject scriptProject, CancellationToken cancellationToken)
    {
        await Task.Yield();

        Directory.CreateDirectory(Path.GetDirectoryName(project.ScriptProjectFileName)!);

        var csprojPath = Path.GetDirectoryName(project.ScriptProjectFileName);
        string xml = scriptProject.GenerateXml(csprojPath);
        await File.WriteAllTextAsync(project.ScriptProjectFileName, xml, cancellationToken);
    }
}
