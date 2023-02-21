// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.BuildSettings;

public record PlatformGroup
{
    private readonly string GroupName;

    private PlatformGroup(string GroupName)
    {
        this.GroupName = GroupName;
    }

    public override string ToString() => GroupName;

    public static readonly PlatformGroup Windows = new("Windows")
    {
    };
}
