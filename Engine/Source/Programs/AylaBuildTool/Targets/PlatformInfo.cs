using System.Runtime.InteropServices;

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

    public static PlatformInfo Current
    {
        get
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                if (RuntimeInformation.OSArchitecture == System.Runtime.InteropServices.Architecture.X64)
                {
                    return Win64;
                }
            }
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            {
                if (RuntimeInformation.OSArchitecture == System.Runtime.InteropServices.Architecture.X64)
                {
                    return Linux64;
                }
            }

            Console.Error.WriteLine("Not supported platform.");
            throw TerminateException.NotSupport();
        }
    }

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

    public static IEnumerable<PlatformInfo> GetAllPlatforms()
    {
        yield return Win64;
        yield return Linux64;
    }
}
