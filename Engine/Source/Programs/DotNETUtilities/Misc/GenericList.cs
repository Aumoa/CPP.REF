// Copyright 2020-2024 Aumoa.lib. All right reserved.

using System.Collections;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;

namespace AylaEngine;

public static class GenericList
{
    private static readonly Dictionary<Type, Type?> s_GenericListCache = new();

    public static bool IsGenericList(this Type type, [MaybeNullWhen(false)] out Type elementType)
    {
        lock (s_GenericListCache)
        {
            if (s_GenericListCache.TryGetValue(type, out elementType) == false)
            {
                if (type.IsGenericType == false)
                {
                    elementType = null;
                    s_GenericListCache.Add(type, elementType);
                    return false;
                }

                Type[] arguments = type.GetGenericArguments();
                if (arguments.Length != 1)
                {
                    elementType = null;
                    s_GenericListCache.Add(type, elementType);
                    return false;
                }

                Type interfaceList = typeof(IList<>).MakeGenericType(arguments);
                if (type.IsAssignableTo(interfaceList) == false)
                {
                    elementType = null;
                    s_GenericListCache.Add(type, elementType);
                    return false;
                }

                elementType = arguments[0];
                s_GenericListCache.Add(type, elementType);
                return true;
            }

            return elementType != null;
        }
    }

    public static object InstantiateList(this Type self)
    {
        if (self.IsInterface)
        {
            if (self.GetGenericTypeDefinition() != typeof(IList<>))
            {
                throw new ArgumentException("Type is interface, but it is not IList<>.");
            }

            Type listType = typeof(List<>).MakeGenericType(self.GetGenericArguments());
            return listType.GetConstructor(Array.Empty<Type>())!.Invoke(Array.Empty<object>());
        }
        else
        {
            ConstructorInfo? constructor = self.GetConstructor(Array.Empty<Type>());
            if (constructor == null)
            {
                throw new ArgumentException("List type cannot be create without arguments.");
            }

            return constructor.Invoke(Array.Empty<object>());
        }
    }

    private static readonly MethodInfo s_AddMethod = typeof(GenericList).GetMethod(nameof(AddTemplate), BindingFlags.Static | BindingFlags.NonPublic)!;

    public static void Add(object self, object? item)
    {
        Type listType = self.GetType();
        MethodInfo instancedMethod = s_AddMethod.MakeGenericMethod(listType.GetGenericArguments());
        instancedMethod.Invoke(null, new object?[] { self, item });
    }

    private static void AddTemplate<T>(this IList<T> self, T item)
    {
        self.Add(item);
    }

    private static readonly MethodInfo s_AddRangeMethod = typeof(GenericList).GetMethod(nameof(AddRangeTemplate), BindingFlags.Static | BindingFlags.NonPublic)!;

    public static void AddRange(object self, object collection)
    {
        Type listType = self.GetType();
        MethodInfo instancedMethod = s_AddRangeMethod.MakeGenericMethod(listType.GetGenericArguments());
        instancedMethod.Invoke(null, new object[] { self, collection });
    }

    private static void AddRangeTemplate<T>(this IList<T> self, IEnumerable collection)
    {
        if (collection is IEnumerable<T> tCollection && self is List<T> list)
        {
            list.AddRange(tCollection);
        }
        else
        {
            foreach (var element in collection)
            {
                self.Add((T)element);
            }
        }
    }
}
