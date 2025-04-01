using System.ComponentModel;
using System.Text;
using AylaEngine;
using CommandLine;
using Spectre.Console;

Console.OutputEncoding = Encoding.UTF8;

CancellationTokenSource cts = new();
Console.CancelKeyPress += OnCancelKeyPress;

if (args.Length > 0)
{
    try
    {
        var pass = Parser.Default.ParseArguments<GenerateOptions, BuildOptions>(args);
        pass = await pass.WithParsedAsync<GenerateOptions>(options => GenerateRunner.RunAsync(options, cts.Token).AsTask());
        pass = await pass.WithParsedAsync<BuildOptions>(options => BuildRunner.RunAsync(options, cts.Token).AsTask());
    }
    catch (TerminateException e)
    {
        return e.ReturnCode;
    }
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

return 0;

void OnCancelKeyPress(object? sender, ConsoleCancelEventArgs args)
{
    args.Cancel = true;
    cts.Cancel();
}