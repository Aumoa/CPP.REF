using CommandLine;

namespace AylaEngine;

[Verb("build")]
internal record BuildOptions
{
    [Option('p', "project", HelpText = "Determines which project set to use. If not specified, the engine project will be used.")]
    public string? ProjectFile { get; init; }

    [Option('t', "target", HelpText = "Determines which target module to build. If not specified, all modules will be built.")]
    public string? Target { get; init; }

    [Option('c', "config", HelpText = "Specifies which configuration option to use during the build.", Default = Configuration.Shipping)]
    public Configuration Config { get; init; } = Configuration.Shipping;

    [Option("editor", HelpText = "Specifies whether to build the editor version of the engine. This will include additional modules and settings for the editor.", Default = false)]
    public bool Editor { get; init; } = false;

    [Option("clean", HelpText = "Specifies the policy for retaining existing output files during the build.", Default = CleanOptions.None)]
    public CleanOptions Clean { get; init; }
}
