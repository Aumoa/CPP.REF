namespace AylaEngine;

public record PlatformInfo
{
    private PlatformInfo()
    {
    }

    public string Name { get; init; } = string.Empty;

    public PlatformGroup Group { get; init; }

    public Architecture Architecture { get; init; }

    public static readonly PlatformInfo Win64 = new()
    {
        Name = "Win64",
        Group = PlatformGroup.Windows,
        Architecture = Architecture.X64
    };
}
