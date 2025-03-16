using System.Text;
using AylaEngine;
using CommandLine;
using Spectre.Console;

Console.OutputEncoding = Encoding.UTF8;

CancellationTokenSource cts = new();
Console.CancelKeyPress += OnCancelKeyPress;

if (args.Length > 0)
{
    var pass = Parser.Default.ParseArguments<GenerateOptions>(args);
    pass = await pass.WithParsedAsync(options => GenerateRunner.RunAsync(options, cts.Token).AsTask());
}
else
{
    var figlet = new FigletText("AylaBuildTool")
        .Justify(Justify.Left);
    AnsiConsole.Write(figlet);

    Commands.Push<MainCommands>();
    while (Commands.TryGetCurrent(out var commands))
    {
        await commands.RenderPromptAsync(cts.Token);
    }
}

return;

void OnCancelKeyPress(object? sender, EventArgs? args)
{
    cts.Cancel();
}