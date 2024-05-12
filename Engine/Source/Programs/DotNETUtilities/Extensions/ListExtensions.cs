// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public static class ListExtensions
{
    public static bool IsValidIndex<T>(this IReadOnlyList<T>? self, int indexOf)
    {
        return self?.Count > indexOf;
    }

    public static int IndexOf<T>(this IReadOnlyList<T>? self, T? element) where T : IEquatable<T>
    {
        if (self is null)
        {
            return -1;
        }

        int indexOf = 0;
        foreach (var item in self)
        {
            if (item.Equals(element))
            {
                return indexOf;
            }

            ++indexOf;
        }

        return -1;
    }

    public static int IndexOf<T>(this IReadOnlyList<T>? self, Func<T, bool> predicate)
    {
        if (self is null)
        {
            return -1;
        }

        int indexOf = 0;
        foreach (var item in self)
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
