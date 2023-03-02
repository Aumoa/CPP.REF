// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Misc;

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

    public void Serialize(BinaryWriter Writer)
    {
        Writer.Write(GroupName);
    }

    public static PlatformGroup Deserialize(BinaryReader Reader)
    {
        return new(Reader.ReadString());
    }
}
