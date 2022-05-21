// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

/// <summary>
/// <see cref="IList{T}"/> 인터페이스 개체에 대한 공통 함수를 제공합니다.
/// </summary>
public static class ListExtensions
{
    /// <summary>
    /// 지정한 인덱스가 대상 리스트 개체에 대해 유효한지 검사합니다.
    /// </summary>
    /// <typeparam name="T"> 리스트 개체의 형식 매개변수를 전달합니다. </typeparam>
    /// <param name="this"> 리스트 개체를 전달합니다. </param>
    /// <param name="indexOf"> 인덱스 번호를 전달합니다. </param>
    /// <returns> 유효할 경우 <see langword="true"/>가, 그 이외의 경우 <see langword="false"/>가 반환됩니다. </returns>
    public static bool IsValidIndex<T>(this IList<T>? @this, int indexOf)
    {
        return @this?.Count > indexOf;
    }

    /// <summary>
    /// 대상 컨테이너에서 지정한 아이템의 인덱스 위치를 가져옵니다.
    /// </summary>
    /// <typeparam name="T"> 형식 매개변수를 전달합니다. </typeparam>
    /// <param name="this"> 컨테이너 개체를 전달합니다. </param>
    /// <param name="predicate"> 찾을 아이템을 전달합니다. </param>
    /// <returns> 대상을 찾았을 경우 대상의 위치에 대한 인덱스가, 찾지 못했을 경우 -1이 반환됩니다. </returns>
    public static int IndexOf<T>(this IList<T>? @this, T? predicate) where T : IEquatable<T>
    {
        if (@this is null)
        {
            return -1;
        }

        int indexOf = 0;
        foreach (var item in @this)
        {
            if (item.Equals(predicate))
            {
                return indexOf;
            }

            ++indexOf;
        }

        return -1;
    }

    /// <summary>
    /// 대상 컨테이너에서 지정한 아이템의 인덱스 위치를 가져옵니다.
    /// </summary>
    /// <typeparam name="T"> 형식 매개변수를 전달합니다. </typeparam>
    /// <param name="this"> 컨테이너 개체를 전달합니다. </param>
    /// <param name="predicate"> 찾을 아이템을 구분할 함수를 전달합니다. </param>
    /// <returns> 대상을 찾았을 경우 대상의 위치에 대한 인덱스가, 찾지 못했을 경우 -1이 반환됩니다. </returns>
    public static int IndexOf<T>(this IList<T>? @this, Func<T, bool> predicate)
    {
        if (@this is null)
        {
            return -1;
        }

        int indexOf = 0;
        foreach (var item in @this)
        {
            if (predicate(item))
            {
                return indexOf;
            }

            ++indexOf;
        }

        return -1;
    }
}
