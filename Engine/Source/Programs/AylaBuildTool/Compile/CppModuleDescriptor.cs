// Copyright 2020-2023 Aumoa.lib. All right reserved.

namespace AE.Compile;

public record CppModuleDescriptor
{
    public required string InterfaceName { get; set; }

    public required string? SourcePath { get; set; }

    public required bool IsInterface { get; set; }

    public required ModuleLookupMethod? LookupMethod { get; set; }

    public static CppModuleDescriptor LoadCacheFromBinary(BinaryReader Reader)
    {
        CppModuleDescriptor Instance = new()
        {
            InterfaceName = Reader.ReadString(),
            SourcePath = Reader.ReadString(),
            IsInterface = Reader.ReadBoolean(),
            LookupMethod = (ModuleLookupMethod)Reader.ReadInt32()
        };
        return Instance;
    }

    public void SaveCacheToBinary(BinaryWriter Writer)
    {
        Writer.Write(InterfaceName);
        Writer.Write(SourcePath ?? string.Empty);
        Writer.Write(IsInterface);
        Writer.Write((int)(LookupMethod ?? 0));
    }
}
