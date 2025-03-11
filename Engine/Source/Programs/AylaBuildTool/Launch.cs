using AylaEngine;
using CommandLine;

CancellationTokenSource cts = new();
Console.CancelKeyPress += OnCancelKeyPress;

var pass = await Parser.Default.ParseArguments<GenerateOptions>(args)
    .WithParsedAsync<GenerateOptions>(async options =>
    {
        string? projectPath = null;
        if (options.ProjectFile != null)
        {
            projectPath = Path.GetDirectoryName(options.ProjectFile);
        }

        var solution = await Solution.ScanProjectsAsync(Global.EngineDirectory, projectPath, cts.Token);
        foreach (var project in solution.AllProjects)
        {
            Console.WriteLine(project);
        }

        var generator = new VisualStudioGenerator();
        await generator.GenerateAsync(solution, cts.Token);
    });

return;

void OnCancelKeyPress(object? sender, EventArgs? args)
{
    cts.Cancel();
}