// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Collections;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;

namespace AE.Misc;

public static class GenericList
{
    public static bool IsGenericList(this Type InType, [MaybeNullWhen(false)] out Type OutElementType)
    {
        if (InType.IsGenericType == false)
        {
            OutElementType = null;
            return false;
        }

        Type[] Arguments = InType.GetGenericArguments();
        if (Arguments.Length != 1)
        {
            OutElementType = null;
            return false;
        }

        Type InterfaceList = typeof(IList<>).MakeGenericType(Arguments);
        if (InType.IsAssignableTo(InterfaceList) == false)
        {
            OutElementType = null;
            return false;
        }

        OutElementType = Arguments[0];
        return true;
    }

    public static object InstantiateList(this Type Self)
    {
        if (Self.IsInterface)
        {
            if (Self.GetGenericTypeDefinition() != typeof(IList<>))
            {
                throw new ArgumentException("Type is interface, but it is not IList<>.");
            }

            Type ListType = typeof(List<>).MakeGenericType(Self.GetGenericArguments());
            return ListType.GetConstructor(Array.Empty<Type>())!.Invoke(Array.Empty<object>());
        }
        else
        {
            ConstructorInfo? Ctor = Self.GetConstructor(Array.Empty<Type>());
            if (Ctor == null)
            {
                throw new ArgumentException("List type cannot be create without arguments.");
            }

            return Ctor.Invoke(Array.Empty<object>());
        }
    }

    public static bool IsValidIndex<T>(this IReadOnlyList<T> Self, Index Index)
    {
        int NumberIndex;
        if (Index.IsFromEnd)
        {
            NumberIndex = Self.Count - Index.Value;
        }
        else
        {
            NumberIndex = Index.Value;
        }
        return Self.Count > NumberIndex;
    }

#pragma warning disable IDE0051
    private static readonly MethodInfo GenericAddMethod = typeof(GenericList).GetMethod("GenericAddTemplate", BindingFlags.Static | BindingFlags.NonPublic)!;

    public static void GenericAdd(this object Self, object? Item)
    {
        Type ListType = Self.GetType();
        MethodInfo InstancedMethod = GenericAddMethod.MakeGenericMethod(ListType.GetGenericArguments());
        InstancedMethod.Invoke(null, new object?[] { Self, Item });
    }

    private static void GenericAddTemplate<T>(this IList<T> Self, T Item)
    {
        Self.Add(Item);
    }

    private static readonly MethodInfo GenericAddRangeMethod = typeof(GenericList).GetMethod("GenericAddRangeTemplate", BindingFlags.Static | BindingFlags.NonPublic)!;

    public static void GenericAddRange(this object Self, object Collection)
    {
        Type ListType = Self.GetType();
        MethodInfo InstancedMethod = GenericAddRangeMethod.MakeGenericMethod(ListType.GetGenericArguments());
        InstancedMethod.Invoke(null, new object[] { Self, Collection });
    }

    private static void GenericAddRangeTemplate<T>(this IList<T> Self, IEnumerable Collection)
    {
        if (Collection is IEnumerable<T> TCollection && Self is List<T> List)
        {
            List.AddRange(TCollection);
        }
        else
        {
            foreach (var Elem in Collection)
            {
                Self.Add((T)Elem);
            }
        }
    }
#pragma warning restore IDE0051
}
