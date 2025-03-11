using System.Runtime.CompilerServices;

namespace AylaEngine;

public static class CollectionExtensions
{
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void AddRange<T>(this ICollection<T> target, IEnumerable<T> values)
    {
        foreach (var value in values)
        {
            target.Add(value);
        }
    }
}
