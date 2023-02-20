// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Misc;

public static class TypeExtensions
{
    public static bool IsIntegral(this Type InType)
    {
        return Type.GetTypeCode(InType) switch
        {
            TypeCode.Byte or
            TypeCode.SByte or
            TypeCode.UInt16 or
            TypeCode.UInt32 or
            TypeCode.UInt64 or
            TypeCode.Int16 or
            TypeCode.Int32 or
            TypeCode.Int64 => true,
            _ => false
        };
    }

    public static bool IsSignedIntegral(this Type InType)
    {
        return Type.GetTypeCode(InType) switch
        {
            TypeCode.SByte or
            TypeCode.Int16 or
            TypeCode.Int32 or
            TypeCode.Int64 => true,
            _ => false
        };
    }

    public static bool IsUnsignedIntegral(this Type InType)
    {
        return Type.GetTypeCode(InType) switch
        {
            TypeCode.Byte or
            TypeCode.UInt16 or
            TypeCode.UInt32 or
            TypeCode.UInt64 => true,
            _ => false
        };
    }

    public static bool IsFloating(this Type InType)
    {
        return Type.GetTypeCode(InType) switch
        {
            TypeCode.Single or
            TypeCode.Double or
            TypeCode.Decimal => true,
            _ => false,
        };
    }

    public static bool IsString(this Type InType)
    {
        return Type.GetTypeCode(InType) == TypeCode.String;
    }

    public static bool IsBoolean(this Type InType)
    {
        return Type.GetTypeCode(InType) == TypeCode.Boolean;
    }
}
