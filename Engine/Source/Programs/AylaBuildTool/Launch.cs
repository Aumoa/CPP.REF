using System.Text;
using AylaEngine;
using CommandLine;

Console.OutputEncoding = Encoding.UTF8;

CancellationTokenSource cts = new();
Console.CancelKeyPress += OnCancelKeyPress;

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

return 0;

void OnCancelKeyPress(object? sender, ConsoleCancelEventArgs args)
{
    args.Cancel = true;
    cts.Cancel();
}