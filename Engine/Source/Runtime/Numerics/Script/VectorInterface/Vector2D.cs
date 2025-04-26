// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Numerics;
using System;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
public struct Vector2D
    : IAdditionOperators<Vector2D, Vector2D, Vector2D>
    , ISubtractionOperators<Vector2D, Vector2D, Vector2D>
    , IMultiplyOperators<Vector2D, double, Vector2D>
    , IDivisionOperators<Vector2D, double, Vector2D>
    , IEqualityOperators<Vector2D, Vector2D, bool>
    , IEquatable<Vector2D>
{
    public double X;
    public double Y;

    public override bool Equals([NotNullWhen(true)] object? obj)
    {
        if (obj is Vector2D v)
        {
            return Equals(v);
        }

        return false;
    }

    public bool Equals(in Vector2D rhs)
    {
        return X == rhs.X && Y == rhs.Y;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y);
    }

    bool IEquatable<Vector2D>.Equals(Vector2D rhs) => Equals(rhs);

    public static Vector2D operator +(in Vector2D lhs, in Vector2D rhs)
    {
        return new Vector2D { X = lhs.X + rhs.X, Y = lhs.Y + rhs.Y };
    }

    static Vector2D IAdditionOperators<Vector2D, Vector2D, Vector2D>.operator +(Vector2D lhs, Vector2D rhs)
        => lhs + rhs;

    public static Vector2D operator -(in Vector2D lhs, in Vector2D rhs)
    {
        return new Vector2D { X = lhs.X - rhs.X, Y = lhs.Y - rhs.Y };
    }

    static Vector2D ISubtractionOperators<Vector2D, Vector2D, Vector2D>.operator -(Vector2D lhs, Vector2D rhs)
        => lhs - rhs;

    public static Vector2D operator *(in Vector2D lhs, double rhs)
    {
        return new Vector2D { X = lhs.X * rhs, Y = lhs.Y * rhs };
    }

    static Vector2D IMultiplyOperators<Vector2D, double, Vector2D>.operator *(Vector2D lhs, double rhs)
        => lhs * rhs;

    public static Vector2D operator *(double lhs, in Vector2D rhs)
    {
        return new Vector2D { X = lhs * rhs.X, Y = lhs * rhs.Y };
    }

    public static Vector2D operator /(in Vector2D lhs, double rhs)
    {
        return new Vector2D { X = lhs.X / rhs, Y = lhs.Y / rhs };
    }

    static Vector2D IDivisionOperators<Vector2D, double, Vector2D>.operator /(Vector2D lhs, double rhs)
        => lhs / rhs;

    public static Vector2D operator /(double lhs, Vector2D rhs)
    {
        return new Vector2D { X = lhs / rhs.X, Y = lhs / rhs.Y };
    }

    public static bool operator ==(in Vector2D lhs, in Vector2D rhs)
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y;
    }

    static bool IEqualityOperators<Vector2D, Vector2D, bool>.operator ==(Vector2D lhs, Vector2D rhs)
        => lhs == rhs;

    public static bool operator !=(in Vector2D lhs, in Vector2D rhs)
    {
        return lhs.X != rhs.X || lhs.Y != rhs.Y;
    }

    static bool IEqualityOperators<Vector2D, Vector2D, bool>.operator !=(Vector2D lhs, Vector2D rhs)
        => lhs != rhs;
}
