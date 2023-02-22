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

    public string ToDefinition() => "PLATFORM_" + GroupName.ToUpper();

    public static readonly PlatformGroup Windows = new("Windows")
    {
    };

    public static readonly PlatformGroup Linux = new("Linux")
    {
    };

    public static void ForEach(Action<PlatformGroup> Body)
    {
        Body(Windows);
        Body(Linux);
    }
}
