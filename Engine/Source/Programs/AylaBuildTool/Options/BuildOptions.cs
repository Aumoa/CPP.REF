using CommandLine;

namespace AylaEngine;

[Verb("build")]
internal record BuildOptions
{
    [Option('p', "project", HelpText = "The project for build.")]
    public string? ProjectFile { get; init; }

    [Option('t', "target", HelpText = "The target to build.")]
    public string? Target { get; init; }

    [Option('c', "config", HelpText = "The configuration of build.", Default = Configuration.Shipping)]
    public Configuration Config { get; init; } = Configuration.Shipping;
}
