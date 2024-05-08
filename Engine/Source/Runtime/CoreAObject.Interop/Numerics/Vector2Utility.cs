using System.Runtime.CompilerServices;

namespace AylaEngine;

public static class Vector2Utility
{
    #region Generators

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T Make<T>(double x, double y) where T : IVector2, new()
    {
        var r = new T();
        r.X = x;
        r.Y = y;
        return r;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T Make<T>(double scalar) where T : IVector2, new()
    {
        var r = new T();
        r.X = scalar;
        r.Y = scalar;
        return r;
    }

    #endregion

    #region Mathematics

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static double Dot<T1, T2>(in T1 lhs, in T2 rhs)
        where T1 : IVector2
        where T2 : IVector2
    {
        return lhs.X * rhs.X + lhs.Y * rhs.Y;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static double Cross<T1, T2>(in T1 lhs, in T2 rhs)
        where T1 : IVector2
        where T2 : IVector2
    {
        return lhs.X * rhs.Y - lhs.Y * rhs.X;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T1 Clamp<T1, T2, T3>(in T1 value, in T2 min, in T3 max)
        where T1 : IVector2, new()
        where T2 : IVector2
        where T3 : IVector2
    {
        return Make<T1>(Math.Clamp(value.X, min.X, max.X), Math.Clamp(value.Y, min.Y, max.Y));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static double GetLengthSq<T>(in T value)
        where T : IVector2
    {
        return value.X * value.X + value.Y * value.Y;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static double GetLength<T>(in T value)
        where T : IVector2
    {
        return Math.Sqrt(GetLengthSq(value));
    }

    #endregion

    #region OPERATORS

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T1 Add<T1, T2>(in T1 lhs, in T2 rhs)
        where T1 : IVector2, new()
        where T2 : IVector2
    {
        return Make<T1>(lhs.X + rhs.X, lhs.Y + rhs.Y);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T1 Subtract<T1, T2>(in T1 lhs, in T2 rhs)
        where T1 : IVector2, new()
        where T2 : IVector2
    {
        return Make<T1>(lhs.X - rhs.X, lhs.Y - rhs.Y);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T1 Multiply<T1, T2>(in T1 lhs, in T2 rhs)
        where T1 : IVector2, new()
        where T2 : IVector2
    {
        return Make<T1>(lhs.X * rhs.X, lhs.Y * rhs.Y);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T Multiply<T>(in T lhs, double rhs)
        where T : IVector2, new()
    {
        return Multiply(lhs, Make<Vector2>(rhs));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T Multiply<T>(double lhs, T rhs)
        where T : IVector2, new()
    {
        return Multiply(Make<T>(lhs), rhs);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T1 Divide<T1, T2>(in T1 lhs, in T2 rhs)
        where T1 : IVector2, new()
        where T2 : IVector2
    {
        return Make<T1>(lhs.X / rhs.X, lhs.Y / rhs.Y);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T Divide<T>(in T lhs, double rhs)
        where T : IVector2, new()
    {
        return Divide(lhs, Make<Vector2>(rhs));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T Divide<T>(double lhs, T rhs)
        where T : IVector2, new()
    {
        return Divide(Make<T>(lhs), rhs);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T1 Mod<T1, T2>(in T1 lhs, in T2 rhs)
        where T1 : IVector2, new()
        where T2 : IVector2
    {
        return Make<T1>(lhs.X % rhs.X, lhs.Y % rhs.Y);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T Mod<T>(in T lhs, double rhs)
        where T : IVector2, new()
    {
        return Mod(lhs, Make<Vector2>(rhs));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T Mod<T>(double lhs, T rhs)
        where T : IVector2, new()
    {
        return Mod(Make<T>(lhs), rhs);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool NearlyEquals<T1, T2>(in T1 lhs, in T2 rhs, double epsilon = 0.0001)
        where T1 : IVector2
        where T2 : IVector2
    {
        return Math.Abs(lhs.X - rhs.X) <= epsilon && Math.Abs(lhs.Y - rhs.Y) <= epsilon;
    }

    #endregion
}
