using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace AylaEngine;

public struct Vector2 : IVector2, IEquatable<Vector2>
{
    public double X;
    public double Y;

    double IVector2.X
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set;
    }

    double IVector2.Y
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override bool Equals(object? rhs)
    {
        if (rhs is Vector2 value)
        {
            return Vector2Utility.NearlyEquals(this, value);
        }
        return false;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public bool Equals(Vector2 rhs)
    {
        return Vector2Utility.NearlyEquals(this, rhs);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator +(in Vector2 lhs, in Vector2 rhs) => Vector2Utility.Add(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator -(in Vector2 lhs, in Vector2 rhs) => Vector2Utility.Subtract(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator *(in Vector2 lhs, in Vector2 rhs) => Vector2Utility.Multiply(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator *(in Vector2 lhs, double rhs) => Vector2Utility.Multiply(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator *(double lhs, in Vector2 rhs) => Vector2Utility.Multiply(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator /(in Vector2 lhs, in Vector2 rhs) => Vector2Utility.Divide(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator /(in Vector2 lhs, double rhs) => Vector2Utility.Divide(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator /(double lhs, in Vector2 rhs) => Vector2Utility.Divide(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator %(in Vector2 lhs, in Vector2 rhs) => Vector2Utility.Mod(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator %(in Vector2 lhs, double rhs) => Vector2Utility.Mod(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Vector2 operator %(double lhs, in Vector2 rhs) => Vector2Utility.Mod(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator ==(in Vector2 lhs, in Vector2 rhs) => Vector2Utility.NearlyEquals(lhs, rhs);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator !=(in Vector2 lhs, in Vector2 rhs) => !Vector2Utility.NearlyEquals(lhs, rhs);
}
