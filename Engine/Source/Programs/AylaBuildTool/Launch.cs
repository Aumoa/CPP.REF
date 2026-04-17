using System.Text;
using AylaEngine;
using CommandLine;

Console.OutputEncoding = Encoding.UTF8;

CancellationTokenSource cts = new();
Console.CancelKeyPress += OnCancelKeyPress;

try
{
    var pass = Parser.Default.ParseArguments<GenerateOptions, BuildOptions, BootstrapOptions>(args);
    pass = await pass.WithParsedAsync<GenerateOptions>(options => GenerateRunner.RunAsync(options, cts.Token).AsTask());
    pass = await pass.WithParsedAsync<BuildOptions>(options => BuildRunner.RunAsync(options, cts.Token).AsTask());
    pass = await pass.WithParsedAsync<BootstrapOptions>(options => BootstrapRunner.RunAsync(options, cts.Token).AsTask());
    return pass.Errors.Any() ? 1 : 0;
}
catch (TerminateException e)
{
    return e.ReturnCode;
}

void OnCancelKeyPress(object? sender, ConsoleCancelEventArgs args)
{
    args.Cancel = true;
    cts.Cancel();
}