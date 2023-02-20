// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Misc;

public static class EnumerableExtensions
{
    public static bool Contains<T>(this IEnumerable<T> Items, Func<T, bool> Predic)
    {
        foreach (var item in Items)
        {
            if (Predic(item))
            {
                return true;
            }
        }

        return false;
    }
}