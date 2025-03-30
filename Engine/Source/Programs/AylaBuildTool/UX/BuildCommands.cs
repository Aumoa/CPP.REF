using Spectre.Console;

namespace AylaEngine;

internal class BuildCommands : Commands
{
    public override async ValueTask RenderPromptAsync(CancellationToken cancellationToken)
    {
        var projectFile = await new TextPrompt<string>("Project Path:")
            .Validate(OnValidateTextPrompt)
            .AllowEmpty()
            .ValidationErrorMessage("The path is not valid project path.")
            .ShowAsync(AnsiConsole.Console, cancellationToken);
        TryConvertProjectPath(projectFile, out projectFile);
        if (string.IsNullOrEmpty(projectFile))
        {
            AnsiConsole.MarkupLine("The [b]engine[/] project selected.", projectFile);
        }
        else
        {
            AnsiConsole.MarkupLine("[b]{0}[/] project selected.", projectFile);
        }

        var target = await new TextPrompt<string?>("Target:")
            .AllowEmpty()
            .ShowAsync(AnsiConsole.Console, cancellationToken);

        var config = await new TextPrompt<Configuration>("Config:")
            .DefaultValue(Configuration.Shipping)
            .ShowAsync(AnsiConsole.Console, cancellationToken);

        var options = new BuildOptions
        {
            ProjectFile = projectFile,
            Target = target,
            Config = config
        };

        await BuildRunner.RunAsync(options, cancellationToken);
        AnsiConsole.MarkupLine("[green]Build completed.[/]");
        Pop();
        return;

        bool OnValidateTextPrompt(string value)
        {
            if (string.IsNullOrEmpty(value))
            {
                return true;
            }

            TryConvertProjectPath(value, out value);
            return File.Exists(value);
        }

        bool TryConvertProjectPath(string value, out string outValue)
        {
            if (Directory.Exists(value))
            {
                outValue = Path.Combine(value, Path.GetFileName(value) + ".aproject");
                return false;
            }

            outValue = value;
            return true;
        }
    }
}
