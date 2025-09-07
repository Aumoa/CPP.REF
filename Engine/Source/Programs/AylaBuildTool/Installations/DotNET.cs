namespace AylaEngine.Installations;

internal static class DotNET
{
    public static async Task BuildAsync(string projectFile, TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        var command = $"build \"{projectFile}\" --verbosity quiet -c \"{VSUtility.GetConfigName(targetInfo)}\" -p:Platform={targetInfo.Platform.Name}";
        Console.WriteLine("dotnet {0}", command);
        var output = await Terminal.ExecuteCommandAsync(command, new Terminal.Options
        {
            Executable = "dotnet",
            Logging = Terminal.Logging.StdOut | Terminal.Logging.StdErr
        }, cancellationToken);
        if (output.ExitCode != 0)
        {
            throw TerminateException.User();
        }
    }
}
