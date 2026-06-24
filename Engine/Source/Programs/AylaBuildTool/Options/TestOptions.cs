using CommandLine;

namespace AylaEngine;

[Verb("test")]
internal record TestOptions
{
    [Option('p', "project", HelpText = "Determines which project set to use. If not specified, the engine project will be used.")]
    public string? ProjectFile { get; init; }

    [Option('t', "target", HelpText = "Determines which test module to build and run.", Default = "AylaUnitTests")]
    public string Target { get; init; } = "AylaUnitTests";

    [Option('c', "config", HelpText = "Specifies which configuration option to use during the build.", Default = Configuration.Shipping)]
    public Configuration Config { get; init; } = Configuration.Shipping;

    [Option("editor", HelpText = "Specifies whether to build the editor version of the engine.", Default = false)]
    public bool Editor { get; init; } = false;

    [Option("skip-shaders", HelpText = "Skips shader compilation and ShaderCompileWorker build.", Default = false)]
    public bool SkipShaders { get; init; } = false;

    [Option('g', "generator")]
    public GeneratorType GeneratorType { get; init; } = GeneratorType.VisualStudio;

    [Value(0, MetaName = "arguments", Required = false, HelpText = "Arguments passed to the test executable.")]
    public IEnumerable<string> Arguments { get; init; } = [];
}
