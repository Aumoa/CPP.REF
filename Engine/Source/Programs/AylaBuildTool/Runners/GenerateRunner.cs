namespace AylaEngine;

internal class GenerateRunner
{
    public static async ValueTask RunAsync(GenerateOptions options, CancellationToken cancellationToken)
    {
        string? projectPath = null;
        if (options.ProjectFile != null)
        {
            projectPath = Path.GetDirectoryName(options.ProjectFile);
        }

        var solution = await Solution.ScanProjectsAsync(Global.EngineDirectory, projectPath, cancellationToken);
        var generator = new VisualStudioGenerator();
        await generator.GenerateAsync(solution, cancellationToken);
    }
}
