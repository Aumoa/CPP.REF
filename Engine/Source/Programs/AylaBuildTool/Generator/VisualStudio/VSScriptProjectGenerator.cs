namespace AylaEngine;

internal static class VSScriptProjectGenerator
{
    public static async ValueTask GenerateAsync(Solution solution, ModuleProject project, CancellationToken cancellationToken)
    {
        await Task.Yield();

        Directory.CreateDirectory(Path.GetDirectoryName(project.ScriptProjectFileName)!);

        string xml = project.ScriptProject.GenerateXml();
        await File.WriteAllTextAsync(project.ScriptProjectFileName, xml, cancellationToken);
    }
}
