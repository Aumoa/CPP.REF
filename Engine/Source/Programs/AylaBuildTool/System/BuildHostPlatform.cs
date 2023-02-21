// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;

namespace AE.System;

public abstract class BuildHostPlatform
{
    private BuildHostPlatform()
    {
    }

    public abstract BuildConfiguration Configuration { get; }

    class WindowsBuildHostPlatform : BuildHostPlatform
    {
        public WindowsBuildHostPlatform()
        {
        }

        public override BuildConfiguration Configuration => new()
        {
            Configuration = BuildSettings.Configuration.Development,
            Platform = TargetPlatform.Win64
        };
    }

    public static readonly BuildHostPlatform WindowsPlatform = new WindowsBuildHostPlatform();

    public static BuildHostPlatform Current =>
        Environment.OSVersion.Platform switch
        {
            PlatformID.Win32NT => WindowsPlatform,
            _ => throw new InvalidOperationException(CoreStrings.Errors.NotSupportedBuildHostPlatform)
        };
}
