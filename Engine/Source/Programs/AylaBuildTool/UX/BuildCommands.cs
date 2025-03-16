using Spectre.Console;

namespace AylaEngine;

internal class BuildCommands : Commands
{
    public override async ValueTask RenderPromptAsync(CancellationToken cancellationToken)
    {
        var prompt = await new TextPrompt<string>("Project Path:")
            .Validate(OnValidateTextPrompt)
            .AllowEmpty()
            .ValidationErrorMessage("The path is not valid project path.")
            .ShowAsync(AnsiConsole.Console, cancellationToken);
        TryConvertProjectPath(prompt, out prompt);
        if (string.IsNullOrEmpty(prompt))
        {
            AnsiConsole.MarkupLine("The [b]engine[/] project selected.", prompt);
        }
        else
        {
            AnsiConsole.MarkupLine("[b]{0}[/] project selected.", prompt);
        }

        var options = new BuildOptions
        {
            ProjectFile = prompt
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
