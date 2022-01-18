// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

public static class GuidHelper
{
    private static int GetImmutableHash(this object Object)
    {
        unchecked
        {
            int HashCode = (int)0xDEADBEEF;

            switch (Object)
            {
                case long Long:
                    return (int)(Long >> 32) ^ (int)Long;
                case ulong Ulong:
                    return (int)(Ulong >> 32) ^ (int)Ulong;
                case int Int:
                    return Int;
                case uint Uint:
                    return (int)Uint;
                case short Short:
                    return Short;
                case ushort Ushort:
                    return Ushort;
                case sbyte Sbyte:
                    return Sbyte;
                case byte Byte:
                    return Byte;
                case char Char:
                    int L = Char >> 8;
                    int R = Char;
                    return (R << 8) | L;

                case string Str:
                    int CharStep = 0;
                    foreach (var Ch in Str)
                    {
                        HashCode ^= Ch.GetImmutableHash() << (CharStep++ % 2 * 16);
                    }
                    break;
                default:
                    if (Object.GetType().IsEnum)
                    {
                        HashCode ^= (int)Object;
                    }
                    else
                    {
                        foreach (var Prop in Object.GetType().GetProperties())
                        {
                            if (!(Prop.GetIndexParameters()?.Length != 0))
                            {
                                HashCode ^= Prop.GetValue(Object)?.GetImmutableHash() ?? 0;
                            }
                        }
                    }
                    break;
            }

            return HashCode;
        }
    }

    public static Guid GenerateGuid(this object Sender, string Seed)
    {
        int Left = Sender.GetImmutableHash();
        int Right = Seed.GetImmutableHash();

        unchecked
        {
            return new Guid((uint)Left, (ushort)(Right >> 16), (ushort)(Right), 0xAE, 0x76, 0xBD, 0x40, 0xA7, 0xB9, 0x2B, 0xCB);
        }
    }
}