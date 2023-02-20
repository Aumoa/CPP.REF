// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Collections.Generic;
using System.Diagnostics;
using System.Text.RegularExpressions;

namespace AE.Misc;

public static class StringUtility
{
    private static readonly Regex IsAlphaRegex = new("[a-z]", RegexOptions.IgnoreCase);

    public static bool IsOnlyAlphabet(this string Text)
    {
        return IsAlphaRegex.IsMatch(Text);
    }

    public static int IndexOfAny(this string Text, IList<string> FindStrings, int StartIndex = 0, int Count = 0, StringComparison Comparison = StringComparison.CurrentCulture)
    {
        Debug.Assert(FindStrings.Contains(p => p.Length == 0));
        List<char> FirstChars = FindStrings.Select(p => p[0]).ToList();

        for (int I = StartIndex; I < Count; I++)
        {
            int IndexOfChar = FirstChars.IndexOf(Text[I]);
            if (IndexOfChar != -1)
            {
                string Compare = FindStrings[IndexOfChar];
                if (Count - I > Compare.Length)
                {
                    continue;
                }

                if (Text.AsSpan(IndexOfChar, Compare.Length).Equals(Compare, Comparison))
                {
                    return I;
                }
            }
        }

        return -1;
    }

    public static int IndexOfAny(this string Text, IEnumerable<string> FindStrings, int StartIndex = 0, int Count = 0, StringComparison Comparison = StringComparison.CurrentCulture)
        => Text.IndexOfAny(FindStrings.ToList(), StartIndex, Count, Comparison);

    public static bool EqualsSpan(this string Text, int IndexOf, ReadOnlySpan<char> Span, StringComparison Comparison = StringComparison.CurrentCulture)
    {
        int Length = Math.Min(Span.Length, Text.Length - IndexOf);
        return Text.AsSpan(IndexOf, Length).Equals(Span, Comparison);
    }

    public static bool IsValidIndex(this string Text, Index IndexOf)
    {
        int FromStart;
        if (IndexOf.IsFromEnd)
        {
            FromStart = Text.Length - IndexOf.Value;
        }
        else
        {
            FromStart = IndexOf.Value;
        }

        return FromStart >= 0 && FromStart < Text.Length;
    }

    public static long GenerateUniqueKey(this string Text)
    {
        const int SPAN = sizeof(long);
        const int BITS = 8;

        long Val = 0;
        long Store = 0;
        for (int I = 0; I < Text.Length; ++I)
        {
            int Loca = I % SPAN;
            byte Byte = (byte)((Text[I] & 0xFF00 >> BITS) ^ (Text[I] & 0xFF));
            Store = Byte << Loca * BITS;
            if (Loca == SPAN - 1)
            {
                Val ^= Store;
                Store = 0;
            }
        }

        Val ^= Store;
        return Val;
    }
}