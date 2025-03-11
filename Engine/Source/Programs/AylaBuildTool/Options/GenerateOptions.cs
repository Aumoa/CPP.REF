using CommandLine;

namespace AylaEngine;

[Verb("generate")]
internal record GenerateOptions
{
    [Option('p', "project", HelpText = "The project for generate project files.")]
    public string? ProjectFile { get; init; }
}
