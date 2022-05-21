// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Reflection;

namespace DotNETUtilities;

/// <summary>
/// <see cref="Type"/> 클래스에 대한 공통 확장 함수를 제공합니다.
/// </summary>
public static class TypeExtensions
{
    /// <summary>
    /// 형식이 정수 계열 형식인지 확인합니다.
    /// </summary>
    /// <param name="type"> 형식 개체를 전달합니다. </param>
    /// <returns> 정수 계열 형식일 경우 <see langword="true"/>를, 그 이외의 경우 <see langword="false"/>를 반환합니다. </returns>
    public static bool IsIntegral(this Type type)
    {
        switch (Type.GetTypeCode(type.GetType()))
        {
            case TypeCode.Byte:
            case TypeCode.SByte:
            case TypeCode.UInt16:
            case TypeCode.UInt32:
            case TypeCode.UInt64:
            case TypeCode.Int16:
            case TypeCode.Int32:
            case TypeCode.Int64:
                return true;
            default:
                return false;
        }
    }

    /// <summary>
    /// 형식이 부호 있는 정수 계열 형식인지 확인합니다.
    /// </summary>
    /// <param name="type"> 형식 개체를 전달합니다. </param>
    /// <returns> 부호 있는 정수 계열 형식일 경우 <see langword="true"/>를, 그 이외의 경우 <see langword="false"/>를 반환합니다. </returns>
    public static bool IsSignedIntegral(this Type type)
    {
        switch (Type.GetTypeCode(type.GetType()))
        {
            case TypeCode.SByte:
            case TypeCode.Int16:
            case TypeCode.Int32:
            case TypeCode.Int64:
                return true;
            default:
                return false;
        }
    }

    /// <summary>
    /// 형식이 부호 없는 정수 계열 형식인지 확인합니다.
    /// </summary>
    /// <param name="type"> 형식 개체를 전달합니다. </param>
    /// <returns> 부호 없는 정수 계열 형식일 경우 <see langword="true"/>를, 그 이외의 경우 <see langword="false"/>를 반환합니다. </returns>
    public static bool IsUnsignedIntegral(this Type type)
    {
        switch (Type.GetTypeCode(type.GetType()))
        {
            case TypeCode.Byte:
            case TypeCode.UInt16:
            case TypeCode.UInt32:
            case TypeCode.UInt64:
                return true;
            default:
                return false;
        }
    }

    /// <summary>
    /// 형식이 문자열 형식인지 확인합니다.
    /// </summary>
    /// <param name="type"> 형식 개체를 전달합니다. </param>
    /// <returns> 문자열 형식일 경우 <see langword="true"/>를, 그 이외의 경우 <see langword="false"/>를 반환합니다. </returns>
    public static bool IsString(this Type type)
    {
        return Type.GetTypeCode(type.GetType()) switch
        {
            TypeCode.String => true,
            _ => false,
        };
    }

    /// <summary>
    /// 형식이 실수 계열 형식인지 확인합니다.
    /// </summary>
    /// <param name="type"> 형식 개체를 전달합니다. </param>
    /// <returns> 실수 계열 형식일 경우 <see langword="true"/>를, 그 이외의 경우 <see langword="false"/>를 반환합니다. </returns>
    public static bool IsFloating(this Type type)
    {
        switch (Type.GetTypeCode(type.GetType()))
        {
            case TypeCode.Single:
            case TypeCode.Double:
            case TypeCode.Decimal:
                return true;
            default:
                return false;
        }
    }

    /// <summary>
    /// 지정 형식이 대상 클래스 형식을 상속하는지 검사합니다.
    /// </summary>
    /// <typeparam name="T"> 대상 클래스 형식을 전달합니다. </typeparam>
    /// <param name="type"> 지정 형식을 전달합니다. </param>
    /// <returns> 대상 클래스 형식을 상속하는지 나타내는 값을 반환합니다. </returns>
    public static bool IsDerivedFrom<T>(this Type type)
    {
        return type.IsSubclassOf(typeof(T));
    }

    /// <summary>
    /// 지정 형식의 <see cref="Type.FullName"/>을 가져옵니다. <see langword="null"/>을 반환할 경우 <see cref="MemberInfo.Name"/>을 가져옵니다.
    /// </summary>
    /// <param name="type"> 지정 형식을 전달합니다. </param>
    /// <returns> <see langword="null"/>이 아닌 이름이 반환됩니다. </returns>
    public static string GetSafeFullName(this Type type)
    {
        return type.FullName ?? type.Name;
    }

    /// <summary>
    /// 형식의 어셈블리 파일 레퍼런스를 가져옵니다.
    /// </summary>
    /// <param name="type"> 형식을 전달합니다. </param>
    /// <returns> 파일 레퍼런스가 반환됩니다. </returns>
    public static FileReference AssemblyLocation(this Type type)
    {
        return new FileReference(type.Assembly.Location);
    }
}
