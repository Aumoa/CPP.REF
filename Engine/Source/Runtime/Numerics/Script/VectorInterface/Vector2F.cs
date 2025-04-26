// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Diagnostics.CodeAnalysis;
using System.Numerics;

namespace Ayla;

public struct Vector2F
    : IAdditionOperators<Vector2F, Vector2F, Vector2F>
    , ISubtractionOperators<Vector2F, Vector2F, Vector2F>
    , IMultiplyOperators<Vector2F, float, Vector2F>
    , IDivisionOperators<Vector2F, float, Vector2F>
    , IEqualityOperators<Vector2F, Vector2F, bool>
    , IEquatable<Vector2F>
{
    public float X;
    public float Y;

    public override bool Equals([NotNullWhen(true)] object? obj)
    {
        if (obj is Vector2F v)
        {
            return Equals(v);
        }

        return false;
    }

    public bool Equals(in Vector2F rhs)
    {
        return X == rhs.X && Y == rhs.Y;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y);
    }

    bool IEquatable<Vector2F>.Equals(Vector2F rhs) => Equals(rhs);

    public static Vector2F operator +(in Vector2F lhs, in Vector2F rhs)
    {
        return new Vector2F { X = lhs.X + rhs.X, Y = lhs.Y + rhs.Y };
    }

    static Vector2F IAdditionOperators<Vector2F, Vector2F, Vector2F>.operator +(Vector2F lhs, Vector2F rhs)
        => lhs + rhs;

    public static Vector2F operator -(in Vector2F lhs, in Vector2F rhs)
    {
        return new Vector2F { X = lhs.X - rhs.X, Y = lhs.Y - rhs.Y };
    }

    static Vector2F ISubtractionOperators<Vector2F, Vector2F, Vector2F>.operator -(Vector2F lhs, Vector2F rhs)
        => lhs - rhs;

    public static Vector2F operator *(in Vector2F lhs, float rhs)
    {
        return new Vector2F { X = lhs.X * rhs, Y = lhs.Y * rhs };
    }

    static Vector2F IMultiplyOperators<Vector2F, float, Vector2F>.operator *(Vector2F lhs, float rhs)
        => lhs * rhs;

    public static Vector2F operator *(float lhs, in Vector2F rhs)
    {
        return new Vector2F { X = lhs * rhs.X, Y = lhs * rhs.Y };
    }

    public static Vector2F operator /(in Vector2F lhs, float rhs)
    {
        return new Vector2F { X = lhs.X / rhs, Y = lhs.Y / rhs };
    }

    static Vector2F IDivisionOperators<Vector2F, float, Vector2F>.operator /(Vector2F lhs, float rhs)
        => lhs / rhs;

    public static Vector2F operator /(float lhs, Vector2F rhs)
    {
        return new Vector2F { X = lhs / rhs.X, Y = lhs / rhs.Y };
    }

    public static bool operator ==(in Vector2F lhs, in Vector2F rhs)
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y;
    }

    static bool IEqualityOperators<Vector2F, Vector2F, bool>.operator ==(Vector2F lhs, Vector2F rhs)
        => lhs == rhs;

    public static bool operator !=(in Vector2F lhs, in Vector2F rhs)
    {
        return lhs.X != rhs.X || lhs.Y != rhs.Y;
    }

    static bool IEqualityOperators<Vector2F, Vector2F, bool>.operator !=(Vector2F lhs, Vector2F rhs)
        => lhs != rhs;
}
