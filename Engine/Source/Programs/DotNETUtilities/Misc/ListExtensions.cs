// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Misc;

public static class ListExtensions
{
    public static bool IsValidIndex<T>(this IList<T>? Self, int IndexOf)
    {
        return Self?.Count > IndexOf;
    }

    public static int IndexOf<T>(this IList<T>? Self, T? Predic) where T : IEquatable<T>
    {
        if (Self is null)
        {
            return -1;
        }

        int IndexOf = 0;
        foreach (var item in Self)
        {
            if (item.Equals(Predic))
            {
                return IndexOf;
            }

            ++IndexOf;
        }

        return -1;
    }

    public static int IndexOf<T>(this IList<T>? Self, Func<T, bool> Predic)
    {
        if (Self is null)
        {
            return -1;
        }

        int IndexOf = 0;
        foreach (var item in Self)
        {
            if (Predic(item))
            {
                return IndexOf;
            }

            ++IndexOf;
        }

        return -1;
    }
}
