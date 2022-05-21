// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

/// <summary>
/// <see cref="IEnumerable{T}"/> 개체에 대한 공통 확장 함수를 제공합니다.
/// </summary>
public static class EnumerableExtensions
{
    /// <summary>
    /// 지정한 조건을 만족하는 개체가 포함되었는지 나타내는 값을 가져옵니다.
    /// </summary>
    /// <typeparam name="T"> 열거 형식의 제네릭 형식을 전달합니다. </typeparam>
    /// <param name="enumerable"> 열거 개체를 전달합니다. </param>
    /// <param name="predic"> 검사식을 전달합니다. </param>
    /// <returns> 개체가 포함되었을 경우 <see langword="true"/>가, 그 이외의 경우 <see langword="false"/>가 반환됩니다. </returns>
    public static bool Contains<T>(this IEnumerable<T> enumerable, Func<T, bool> predic)
    {
        foreach (var item in enumerable)
        {
            if (predic(item))
            {
                return true;
            }
        }

        return false;
    }
}