using System.Diagnostics;

namespace AylaEngine;

internal class GenerateRunner
{
    public static async ValueTask RunAsync(GenerateOptions options, CancellationToken cancellationToken)
    {
        var solution = await SolutionLoader.CreateDefault().LoadAsync(Global.EngineDirectory, options.ProjectFile, cancellationToken);

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
