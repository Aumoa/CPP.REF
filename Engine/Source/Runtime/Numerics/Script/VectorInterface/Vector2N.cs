// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
public struct Vector2N
    : IAdditionOperators<Vector2N, Vector2N, Vector2N>
    , ISubtractionOperators<Vector2N, Vector2N, Vector2N>
    , IMultiplyOperators<Vector2N, int, Vector2N>
    , IMultiplyOperators<Vector2N, float, Vector2F>
    , IMultiplyOperators<Vector2N, double, Vector2D>
    , IDivisionOperators<Vector2N, int, Vector2N>
    , IDivisionOperators<Vector2N, float, Vector2F>
    , IDivisionOperators<Vector2N, double, Vector2D>
    , IEqualityOperators<Vector2N, Vector2N, bool>
    , IEquatable<Vector2N>
{
    public int X;
    public int Y;

    public override bool Equals([NotNullWhen(true)] object? obj)
    {
        if (obj is Vector2N v)
        {
            return Equals(v);
        }

        return false;
    }

    public bool Equals(in Vector2N rhs)
    {
        return X == rhs.X && Y == rhs.Y;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y);
    }

    bool IEquatable<Vector2N>.Equals(Vector2N rhs) => Equals(rhs);

    public static Vector2N operator +(in Vector2N lhs, in Vector2N rhs)
    {
        return new Vector2N { X = lhs.X + rhs.X, Y = lhs.Y + rhs.Y };
    }

    static Vector2N IAdditionOperators<Vector2N, Vector2N, Vector2N>.operator +(Vector2N lhs, Vector2N rhs)
        => lhs + rhs;

    public static Vector2N operator -(in Vector2N lhs, in Vector2N rhs)
    {
        return new Vector2N { X = lhs.X - rhs.X, Y = lhs.Y - rhs.Y };
    }

    static Vector2N ISubtractionOperators<Vector2N, Vector2N, Vector2N>.operator -(Vector2N lhs, Vector2N rhs)
        => lhs - rhs;

    public static Vector2N operator *(in Vector2N lhs, int rhs)
    {
        return new Vector2N { X = lhs.X * rhs, Y = lhs.Y * rhs };
    }

    static Vector2N IMultiplyOperators<Vector2N, int, Vector2N>.operator *(Vector2N lhs, int rhs)
        => lhs * rhs;

    public static Vector2F operator *(in Vector2N lhs, float rhs)
    {
        return new Vector2F { X = lhs.X * rhs, Y = lhs.Y * rhs };
    }

    static Vector2F IMultiplyOperators<Vector2N, float, Vector2F>.operator *(Vector2N lhs, float rhs)
        => lhs * rhs;

    public static Vector2D operator *(in Vector2N lhs, double rhs)
    {
        return new Vector2D { X = lhs.X * rhs, Y = lhs.Y * rhs };
    }

    static Vector2D IMultiplyOperators<Vector2N, double, Vector2D>.operator *(Vector2N lhs, double rhs)
        => lhs * rhs;

    public static Vector2N operator *(int lhs, in Vector2N rhs)
    {
        return new Vector2N { X = lhs * rhs.X, Y = lhs * rhs.Y };
    }

    public static Vector2N operator /(in Vector2N lhs, int rhs)
    {
        return new Vector2N { X = lhs.X / rhs, Y = lhs.Y / rhs };
    }

    static Vector2N IDivisionOperators<Vector2N, int, Vector2N>.operator /(Vector2N lhs, int rhs)
        => lhs / rhs;

    public static Vector2F operator /(in Vector2N lhs, float rhs)
    {
        return new Vector2F { X = lhs.X * rhs, Y = lhs.Y * rhs };
    }

    static Vector2F IDivisionOperators<Vector2N, float, Vector2F>.operator /(Vector2N lhs, float rhs)
        => lhs * rhs;

    public static Vector2D operator /(in Vector2N lhs, double rhs)
    {
        return new Vector2D { X = lhs.X * rhs, Y = lhs.Y * rhs };
    }

    static Vector2D IDivisionOperators<Vector2N, double, Vector2D>.operator /(Vector2N lhs, double rhs)
        => lhs * rhs;

    public static Vector2N operator /(int lhs, Vector2N rhs)
    {
        return new Vector2N { X = lhs / rhs.X, Y = lhs / rhs.Y };
    }

    public static bool operator ==(in Vector2N lhs, in Vector2N rhs)
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y;
    }

    static bool IEqualityOperators<Vector2N, Vector2N, bool>.operator ==(Vector2N lhs, Vector2N rhs)
        => lhs == rhs;

    public static bool operator !=(in Vector2N lhs, in Vector2N rhs)
    {
        return lhs.X != rhs.X || lhs.Y != rhs.Y;
    }

    static bool IEqualityOperators<Vector2N, Vector2N, bool>.operator !=(Vector2N lhs, Vector2N rhs)
        => lhs != rhs;
}
