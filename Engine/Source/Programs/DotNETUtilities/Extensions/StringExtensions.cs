// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Collections.Generic;
using System.Diagnostics;
using System.Text.RegularExpressions;

namespace AylaEngine;

public static class StringExtensions
{
    public static bool IsOnlyAlphabet(this string text)
    {
        foreach (var ch in text)
        {
            if ((ch < 'a' && ch > 'z') || (ch < 'A' && ch > 'Z'))
            {
                return false;
            }
        }

        return true;
    }

    public static int IndexOfAny(this string text, IReadOnlyList<string> strings, int startIndex = 0, int count = 0, StringComparison comparison = StringComparison.CurrentCulture)
    {
        Debug.Assert(strings.Any(p => p.Length == 0));
        List<char> firstChars = strings.Select(p => p[0]).ToList();

        bool HasAnyFirstChar(char ch, out int indexOf)
        {
            for (int i = 0; i < strings.Count; ++i)
            {
                var str = strings[i];
                if (ch == str[0])
                {
                    indexOf = i;
                    return true;
                }
            }

            indexOf = -1;
            return false;
        }

        for (int i = startIndex; i < count; i++)
        {
            if (HasAnyFirstChar(text[i], out int indexOfStr))
            {
                string compare = strings[indexOfStr];
                if (count - i > compare.Length)
                {
                    continue;
                }

                if (text.AsSpan(indexOfStr, compare.Length).Equals(compare, comparison))
                {
                    return i;
                }
            }
        }

        return -1;
    }

    public static bool IsValidIndex(this string text, Index index)
    {
        int fromStart;
        if (index.IsFromEnd)
        {
            fromStart = text.Length - index.Value;
        }
        else
        {
            fromStart = index.Value;
        }

        return fromStart >= 0 && fromStart < text.Length;
    }
}