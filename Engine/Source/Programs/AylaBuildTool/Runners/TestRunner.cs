namespace AylaEngine;

internal static class TestRunner
{
    public static async ValueTask RunAsync(TestOptions options, CancellationToken cancellationToken)
    {
        var targetName = string.IsNullOrWhiteSpace(options.Target) ? "AylaUnitTests" : options.Target;

        await BuildRunner.RunAsync(new BuildOptions
        {
            ProjectFile = options.ProjectFile,
            Target = targetName,
            Config = options.Config,
            Editor = options.Editor,
            SkipShaders = options.SkipShaders,
            GeneratorType = options.GeneratorType
        }, cancellationToken);

        var targetInfo = BuildRulesExceptionHandler.Evaluate(
            "Failed to create build target information.",
            () => TargetInfo.CreateDefaultTargetInfo(options.Config, options.Editor));
        var solution = await SolutionLoader.CreateDefault().LoadAsync(Global.EngineDirectory, options.ProjectFile, cancellationToken);
        var targetProject = solution.FindProject(targetName);
        if (targetProject is not ModuleProject moduleProject)
        {
            Console.Error.WriteLine("Test target '{0}' is not a module project.", targetName);
            throw TerminateException.User();
        }

        var rule = moduleProject.GetRule(targetInfo);
        if (rule.Type is not (ModuleType.Console or ModuleType.Application))
        {
            Console.Error.WriteLine("Test target '{0}' must be a console or application module.", targetName);
            throw TerminateException.User();
        }

        var installation = Installation.CreateDefaultInstallation();
        var executableFileName = moduleProject.Group.OutputFileName(
            installation,
            targetInfo,
            moduleProject.Name,
            rule.Type,
            FolderPolicy.PathType.Current);
        if (File.Exists(executableFileName) == false)
        {
            Console.Error.WriteLine("Test executable was not found: {0}", executableFileName);
            throw TerminateException.Abort();
        }

        Console.WriteLine("Running test target '{0}'...", targetName);
        var output = await Terminal.ExecuteCommandAsync(
            string.Join(' ', options.Arguments.Select(EscapeArgument)),
            new Terminal.Options
            {
                Executable = executableFileName,
                WorkingDirectory = Path.GetDirectoryName(executableFileName)!,
                Logging = Terminal.Logging.All
            },
            cancellationToken);

        if (output.ExitCode != 0)
        {
            throw TerminateException.User("Test target '" + targetName + "' failed with exit code " + output.ExitCode + ".");
        }

        Console.WriteLine("Test target '{0}' passed.", targetName);
    }

    private static string EscapeArgument(string value)
    {
        if (value.Length == 0)
        {
            return "\"\"";
        }

        if (value.Any(char.IsWhiteSpace) == false && value.Contains('"') == false)
        {
            return value;
        }

        return "\"" + value.Replace("\\", "\\\\").Replace("\"", "\\\"") + "\"";
    }
}
