using System.Runtime.InteropServices;

namespace AylaEngine;

public record TargetInfo : ITargetInfo
{
    public PlatformInfo Platform { get; init; } = PlatformInfo.Win64;

    public Configuration Config { get; init; } = Configuration.Shipping;

    public bool Editor { get; init; }

    public static readonly ITargetInfo Environment;

    static TargetInfo()
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            Environment = new TargetInfo
            {
                Platform = PlatformInfo.Win64,
                Config = Configuration.Development,
                Editor = true
            };
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            Environment = new TargetInfo
            {
                Platform = PlatformInfo.Linux64,
                Config = Configuration.Development,
                Editor = true
            };
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            Environment = new TargetInfo
            {
                Platform = PlatformInfo.OSXArm64,
                Config = Configuration.Development,
                Editor = true
            };
        }
        else
        {
            throw new PlatformNotSupportedException($"TargetInfo: Not supported platform. {RuntimeInformation.OSDescription}.");
        }
    }

    public static IEnumerable<TargetInfo> GetAllTargets()
    {
        return InnerGet(PlatformInfo.Win64, true)
            .Concat(InnerGet(PlatformInfo.Win64, false))
            .Concat(InnerGet(PlatformInfo.Linux64, true))
            .Concat(InnerGet(PlatformInfo.Linux64, false))
            .Concat(InnerGet(PlatformInfo.OSXArm64, true))
            .Concat(InnerGet(PlatformInfo.OSXArm64, false));

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

    public static TargetInfo CreateDefaultTargetInfo(Configuration config, bool editor)
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            return new TargetInfo
            {
                Platform = PlatformInfo.Win64,
                Editor = editor,
                Config = config
            };
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            return new TargetInfo
            {
                Platform = PlatformInfo.Linux64,
                Editor = editor,
                Config = config
            };
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            return new TargetInfo
            {
                Platform = PlatformInfo.OSXArm64,
                Editor = editor,
                Config = config
            };
        }
        else
        {
            throw new PlatformNotSupportedException($"TargetInfo: Not supported platform. {RuntimeInformation.OSDescription}.");
        }
    }
}
