// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using Spectre.Console;

namespace AylaEngine;

internal class DotNETCompiler
{
    private static SemaphoreSlim m_Access = new(1);

    public async Task<Terminal.Output> PublishAsync(string projectFile, TargetInfo targetInfo, string outputDir, CancellationToken cancellationToken = default)
    {
        await m_Access.WaitAsync(cancellationToken);
        try
        {
            var config = VSUtility.GetConfigName(targetInfo);
            var platform = VSUtility.GetArchitectureName(targetInfo);
            var output = await Terminal.ExecuteCommandAsync($"build {projectFile} -c \"{config}\" /p:Platform=\"{platform}\" --nologo", new Terminal.Options
            {
                Executable = "dotnet",
                Logging = Terminal.Logging.None,
                WorkingDirectory = Path.GetDirectoryName(projectFile) ?? string.Empty,
            }, cancellationToken);

            if (output.IsCompletedSuccessfully)
            {
                var outputDll = Path.Combine(outputDir, Path.GetFileNameWithoutExtension(projectFile) + ".dll");
                if (ConsoleEnvironment.IsDynamic)
                {
                    AnsiConsole.MarkupLine("[green]{0} generated.[/]", outputDll);
                }
                else
                {
                    Console.WriteLine("{0} generated.", outputDll);
                }
            }
            else
            {
                if (ConsoleEnvironment.IsDynamic)
                {
                    AnsiConsole.MarkupLine("[red]{0}[/]", string.Join('\n', output.Logs.Select(p => p.Value)).EscapeMarkup());
                }
                else
                {
                    Console.WriteLine(string.Join('\n', output.Logs.Select(p => p.Value)));
                }
            }

            return output;
        }
        finally
        {
            m_Access.Release();
        }
    }
}
