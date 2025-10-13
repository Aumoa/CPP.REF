using System.Runtime.InteropServices;

namespace AylaEngine;

internal record TargetInfo : ITargetInfo
{
    public PlatformInfo Platform { get; init; } = PlatformInfo.Win64;

    public Configuration Config { get; init; } = Configuration.Shipping;

    public bool Editor { get; init; }

    public static readonly TargetInfo Default = new()
    {
        Platform = PlatformInfo.Win64,
        Config = Configuration.Shipping,
        Editor = false
    };

    public static IEnumerable<TargetInfo> GetAllTargets()
    {
        return InnerGet(PlatformInfo.Win64, true)
            .Concat(InnerGet(PlatformInfo.Win64, false))
            .Concat(InnerGet(PlatformInfo.Linux64, true))
            .Concat(InnerGet(PlatformInfo.Linux64, false));

        IEnumerable<TargetInfo> InnerGet(PlatformInfo platform, bool editor)
        {
            foreach (var config in Enum.GetValues<Configuration>())
            {
                yield return new TargetInfo
                {
                    Platform = platform,
                    Config = config,
                    Editor = editor
                };
            }
        }
    }

    public static TargetInfo CreateDefaultTargetInfo(BuildOptions options)
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            return new TargetInfo
            {
                Platform = PlatformInfo.Win64,
                Editor = options.Editor,
                Config = options.Config
            };
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            return new TargetInfo
            {
                Platform = PlatformInfo.Linux64,
                Editor = options.Editor,
                Config = options.Config
            };
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            return new TargetInfo
            {
                Platform = PlatformInfo.OSX64,
                Editor = options.Editor,
                Config = options.Config
            };
        }
        else
        {
            Console.Error.WriteLine("Not supported platform.");
            throw TerminateException.Internal();
        }
    }
}
