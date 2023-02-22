// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;

using System.Runtime.InteropServices;

namespace AE.System;

public abstract class BuildHostPlatform
{
    private BuildHostPlatform()
    {
    }

    public abstract TargetPlatform Platform { get; }

    class WindowsBuildHostPlatform : BuildHostPlatform
    {
        public WindowsBuildHostPlatform()
        {
        }

        public override TargetPlatform Platform => TargetPlatform.Win64;
    }

    public static readonly BuildHostPlatform WindowsPlatform = new WindowsBuildHostPlatform();

    class LinuxBuildHostPlatform : BuildHostPlatform
    {
        public LinuxBuildHostPlatform()
        {
        }

        public override TargetPlatform Platform => TargetPlatform.Linux;
    }

    public static readonly BuildHostPlatform LinuxPlatform = new LinuxBuildHostPlatform();

    public static BuildHostPlatform Current
    {
        get
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                return RuntimeInformation.OSArchitecture switch
                {
                    Architecture.X64 => WindowsPlatform,
                    _ => throw new InvalidOperationException(CoreStrings.Errors.NotSupportedBuildHostPlatform)
                };
            }
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            {
                return RuntimeInformation.OSArchitecture switch
                {
                    Architecture.X64 => LinuxPlatform,
                    _ => throw new InvalidOperationException(CoreStrings.Errors.NotSupportedBuildHostPlatform)
                };
            }
            else
            {
                throw new InvalidOperationException(CoreStrings.Errors.NotSupportedBuildHostPlatform);
            }
        }
    }
}
