
namespace AylaEngine;

internal class VSCSolutionGenerator : Generator
{
    public override async ValueTask GenerateAsync(Solution solution, CancellationToken cancellationToken = default)
    {
        List<Task> tasks = [];
        foreach (var project in solution.Projects.OfType<ModuleProject>())
        {
            tasks.Add(VSCCppProjectGenerator.GenerateAsync(solution, project, cancellationToken).AsTask());
        }

        await Task.WhenAll(tasks);
    }
}
