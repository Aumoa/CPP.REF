using CommandLine;

namespace AylaEngine;

[Verb("build")]
internal record BuildOptions
{
    [Option('p', "project", HelpText = "The project for build.")]
    public string? ProjectFile { get; init; }
}
