namespace AylaEngine;

public record PlatformInfo
{
    private PlatformInfo()
    {
    }

    public string Name { get; init; } = string.Empty;

    public PlatformGroup Group { get; init; }

    public Architecture Architecture { get; init; }

    public override string ToString() => Name;

    public static readonly PlatformInfo Win64 = new()
    {
        Name = "Win64",
        Group = PlatformGroup.Windows,
        Architecture = Architecture.X64
    };

    public static readonly PlatformInfo Linux64 = new()
    {
        Name = "Linux64",
        Group = PlatformGroup.Linux,
        Architecture = Architecture.X64
    };
}
