// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Collections.Frozen;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;

namespace Ayla;

/// <summary>
/// Provide various utilities to facilitate smooth C# Reflection processing.
/// </summary>
public static class ReflectionUtility
{
    public enum FindResult
    {
        Success,
        NotFound,
        Ambiguous,
    }

    private static class Nested
    {
        public static readonly Type[] s_All;
        public static readonly FrozenDictionary<string, int?> s_FullNameMap;

        static Nested()
        {
            s_All = AppDomain.CurrentDomain.GetAssemblies().SelectMany(p => p.GetTypes()).ToArray();
            var fullNameMap = new Dictionary<string, int?>(s_All.Length);
            for (int i = 0; i < s_All.Length; ++i)
            {
                var type = s_All[i];
                if (type.FullName == null)
                {
                    // Cases where FullName returns null, such as generic, are simply not included in the Dictionary.
                    continue;
                }

                if (fullNameMap.TryAdd(type.FullName, i) == false)
                {
                    // This case involves duplicate FullName values. The key is considered to exist, but it is set to null to allow conflict detection.
                    fullNameMap[type.FullName] = null;
                }
            }

            s_FullNameMap = fullNameMap.ToFrozenDictionary();
        }

        /// <summary>
        /// Find the type corresponding to <paramref name="fullName"/> from the list of all currently loaded assemblies.
        /// </summary>
        public static FindResult FindTypeSimple(string fullName, [MaybeNullWhen(false)] out Type? result)
        {
            if (s_FullNameMap.TryGetValue(fullName, out var index))
            {
                if (index.HasValue == false)
                {
                    // The type is ambiguous.
                    result = null;
                    return FindResult.Ambiguous;
                }
                else
                {
                    // The type is found.
                    result = s_All[index.Value];
                    return FindResult.Success;
                }
            }
            else
            {
                // The type is not found.
                result = null;
                return FindResult.NotFound;
            }
        }
    }
}
