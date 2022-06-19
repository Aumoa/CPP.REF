﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

namespace DotNETUtilities;

/// <summary>
/// <see cref="string"/> 개체에 대한 공통 확장 함수를 제공합니다.
/// </summary>
public static class StringUtility
{
    /// <summary>
    /// 지정한 문자열에서 대상 문자열 중 최초로 발견된 문자열의 리스트 인덱스를 찾습니다. 없을 경우 -1을 반환합니다.
    /// </summary>
    /// <param name="text"> 문자열을 전달합니다. </param>
    /// <param name="findStrs"> 찾을 문자열 목록을 전달합니다. </param>
    /// <param name="startIndex"> 문자열을 찾기 시작할 위치를 전달합니다. </param>
    /// <param name="count"> 찾을 문자열의 길이를 전달합니다. </param>
    /// <param name="comparison"> 문자열 찾기 옵션을 전달합니다. </param>
    /// <returns> 찾은 문자열의 리스트 인덱스를 전달합니다. </returns>
    public static int IndexOfAny(this string text, IList<string> findStrs, int startIndex = 0, int count = 0, StringComparison comparison = StringComparison.CurrentCulture)
    {
        Debug.Assert(findStrs.Contains(p => p.Length == 0));
        List<char> firstChars = findStrs.Select(p => p[0]).ToList();

        for (int i = startIndex; i < count; i++)
        {
            int indexOfChar = firstChars.IndexOf(text[i]);
            if (indexOfChar != -1)
            {
                string compare = findStrs[indexOfChar];
                if (count - i > compare.Length)
                {
                    continue;
                }

                if (text.AsSpan(indexOfChar, compare.Length).Equals(compare, comparison))
                {
                    return i;
                }
            }
        }

        return -1;
    }

    /// <summary>
    /// 지정한 문자열에서 대상 문자열 중 최초로 발견된 문자열의 리스트 인덱스를 찾습니다. 없을 경우 -1을 반환합니다.
    /// </summary>
    /// <param name="text"> 문자열을 전달합니다. </param>
    /// <param name="findStrs"> 찾을 문자열 목록을 전달합니다. </param>
    /// <param name="startIndex"> 문자열을 찾기 시작할 위치를 전달합니다. </param>
    /// <param name="count"> 찾을 문자열의 길이를 전달합니다. </param>
    /// <param name="comparison"> 문자열 찾기 옵션을 전달합니다. </param>
    /// <returns> 찾은 문자열의 리스트 인덱스를 전달합니다. </returns>
    public static int IndexOfAny(this string text, IEnumerable<string> findStrs, int startIndex = 0, int count = 0, StringComparison comparison = StringComparison.CurrentCulture)
        => IndexOfAny(text, findStrs.ToList(), startIndex, count, comparison);

    /// <summary>
    /// 지정한 문자열에서 특정 부분을 대상 문자 범위와 비교합니다.
    /// </summary>
    /// <param name="text"> 문자열을 전달합니다. </param>
    /// <param name="indexOf"> 비교 시작 위치를 전달합니다. </param>
    /// <param name="span"> 비교 대상 문자 범위를 전달합니다. </param>
    /// <param name="comparison"> 비교 메서드를 전달합니다. </param>
    /// <returns> 비교 결과가 반환됩니다. </returns>
    public static bool EqualsSpan(this string text, int indexOf, ReadOnlySpan<char> span, StringComparison comparison = StringComparison.CurrentCulture)
    {
        int length = Math.Min(span.Length, text.Length - indexOf);
        return text.AsSpan(indexOf, length).Equals(span, comparison);
    }

    /// <summary>
    /// 지정한 문자열에서 특정 위치가 유효한 위치인지 검사합니다.
    /// </summary>
    /// <param name="text"> 문자열을 전달합니다. </param>
    /// <param name="indexOf"> 검사 위치를 전달합니다. </param>
    /// <returns> 유효 상태를 나타내는 값이 반환됩니다. </returns>
    public static bool IsValidIndex(this string text, Index indexOf)
    {
        int fromStart;
        if (indexOf.IsFromEnd)
        {
            fromStart = text.Length - indexOf.Value;
        }
        else
        {
            fromStart = indexOf.Value;
        }

        return fromStart >= 0 && fromStart < text.Length;
    }
}