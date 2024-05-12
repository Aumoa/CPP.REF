// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public class PlatformGroup
{
    public string GroupName { get; set; } = string.Empty;

    public string ToDefinition() => "PLATFORM_" + GroupName.ToUpper();

    public static readonly PlatformGroup Windows = new()
    {
        GroupName = "Windows"
    };

    public static readonly PlatformGroup Linux = new()
    {
        GroupName = "Linux"
    };

    public static void ForEach(Action<PlatformGroup> Body)
    {
        Body(Windows);
        Body(Linux);
    }
}
