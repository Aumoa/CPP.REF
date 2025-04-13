// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

internal class DotNETCompiler
{
    private static SemaphoreSlim m_Access = new(1);

    public async Task<Terminal.Output> PublishAsync(string projectFile, string config, string platform, string outputDir, CancellationToken cancellationToken = default)
    {
        await m_Access.WaitAsync(cancellationToken);
        try
        {
            return await Terminal.ExecuteCommandAsync($"publish {projectFile} -c \"{config}\" /p:Platform=\"{platform}\" --nologo -o \"{outputDir}\"", new Terminal.Options
            {
                Executable = "dotnet",
                Logging = Terminal.Logging.None,
                WorkingDirectory = Path.GetDirectoryName(projectFile) ?? string.Empty,
            }, cancellationToken);
        }
        finally
        {
            m_Access.Release();
        }
    }
}
