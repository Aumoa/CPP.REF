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
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
            {
                if (RuntimeInformation.OSArchitecture == System.Runtime.InteropServices.Architecture.Arm64)
                {
                    return OSXArm64;
                }
            }

            Console.Error.WriteLine("PlatformInfo: Not supported platform. {0} ({1})", RuntimeInformation.OSDescription, RuntimeInformation.ProcessArchitecture);
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

    public static readonly PlatformInfo OSXArm64 = new()
    {
        Name = "OSXArm64",
        Group = PlatformGroup.OSX,
        Architecture = Architecture.Arm64
    };

    public static IEnumerable<PlatformInfo> GetAllPlatforms()
    {
        yield return Win64;
        yield return Linux64;
        yield return OSXArm64;
    }
}
