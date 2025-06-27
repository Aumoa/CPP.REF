using System.Diagnostics;

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

        Generator generator;
        switch (options.GeneratorType)
        {
            case GeneratorType.VisualStudio:
                generator = new VSSolutionGenerator();
                break;
            case GeneratorType.VisualStudioCode:
                generator = new VSCSolutionGenerator();
                break;
            default:
                Console.Error.WriteLine("Generator type('{0}') not supported.", options.GeneratorType);
                throw TerminateException.User();
        };

        await generator.GenerateAsync(solution, cancellationToken);
    }
}
