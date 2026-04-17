using CommandLine;

namespace AylaEngine;

[Verb("bootstrap", HelpText = "Bootstrap a new game project with minimal required files.")]
internal record BootstrapOptions
{
    [Option('p', "project", Required = true, HelpText = "The name of the project to create.")]
    public required string ProjectName { get; init; }

    [Option('t', "target", Default = ".", HelpText = "The target directory where the project will be created.")]
    public string TargetDirectory { get; init; } = ".";

    [Option('g', "generator")]
    public GeneratorType GeneratorType { get; init; } = GeneratorType.VisualStudio;
}
