// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
public struct Vector4N
    : IAdditionOperators<Vector4N, Vector4N, Vector4N>
    , ISubtractionOperators<Vector4N, Vector4N, Vector4N>
    , IMultiplyOperators<Vector4N, int, Vector4N>
    , IMultiplyOperators<Vector4N, float, Vector4F>
    , IMultiplyOperators<Vector4N, double, Vector4D>
    , IDivisionOperators<Vector4N, int, Vector4N>
    , IDivisionOperators<Vector4N, float, Vector4F>
    , IDivisionOperators<Vector4N, double, Vector4D>
    , IEqualityOperators<Vector4N, Vector4N, bool>
    , IEquatable<Vector4N>
{
    public int X;
    public int Y;
    public int Z;
    public int W;

    public override bool Equals([NotNullWhen(true)] object? obj)
    {
        if (obj is Vector4N v)
        {
            return Equals(v);
        }

        return false;
    }

    public bool Equals(in Vector4N rhs)
    {
        return X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y, Z, W);
    }

    bool IEquatable<Vector4N>.Equals(Vector4N rhs) => Equals(rhs);

    public static Vector4N operator +(in Vector4N lhs, in Vector4N rhs)
    {
        return new Vector4N { X = lhs.X + rhs.X, Y = lhs.Y + rhs.Y, Z = lhs.Z + rhs.Z, W = lhs.W + rhs.W };
    }

    static Vector4N IAdditionOperators<Vector4N, Vector4N, Vector4N>.operator +(Vector4N lhs, Vector4N rhs)
        => lhs + rhs;

    public static Vector4N operator -(in Vector4N lhs, in Vector4N rhs)
    {
        return new Vector4N { X = lhs.X - rhs.X, Y = lhs.Y - rhs.Y, Z = lhs.Z - rhs.Z, W = lhs.W - rhs.W };
    }

    static Vector4N ISubtractionOperators<Vector4N, Vector4N, Vector4N>.operator -(Vector4N lhs, Vector4N rhs)
        => lhs - rhs;

    public static Vector4N operator *(in Vector4N lhs, int rhs)
    {
        return new Vector4N { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs, W = lhs.W * rhs };
    }

    static Vector4N IMultiplyOperators<Vector4N, int, Vector4N>.operator *(Vector4N lhs, int rhs)
        => lhs * rhs;

    public static Vector4F operator *(in Vector4N lhs, float rhs)
    {
        return new Vector4F { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs, W = lhs.W * rhs };
    }

    static Vector4F IMultiplyOperators<Vector4N, float, Vector4F>.operator *(Vector4N lhs, float rhs)
        => lhs * rhs;

    public static Vector4D operator *(in Vector4N lhs, double rhs)
    {
        return new Vector4D { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs, W = lhs.W * rhs };
    }

    static Vector4D IMultiplyOperators<Vector4N, double, Vector4D>.operator *(Vector4N lhs, double rhs)
        => lhs * rhs;

    public static Vector4N operator *(int lhs, in Vector4N rhs)
    {
        return new Vector4N { X = lhs * rhs.X, Y = lhs * rhs.Y, Z = lhs * rhs.Z, W = lhs * rhs.W };
    }

    public static Vector4N operator /(in Vector4N lhs, int rhs)
    {
        return new Vector4N { X = lhs.X / rhs, Y = lhs.Y / rhs, Z = lhs.Z / rhs, W = lhs.W / rhs };
    }

    static Vector4N IDivisionOperators<Vector4N, int, Vector4N>.operator /(Vector4N lhs, int rhs)
        => lhs / rhs;

    public static Vector4F operator /(in Vector4N lhs, float rhs)
    {
        return new Vector4F { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs, W = lhs.W * rhs };
    }

    static Vector4F IDivisionOperators<Vector4N, float, Vector4F>.operator /(Vector4N lhs, float rhs)
        => lhs * rhs;

    public static Vector4D operator /(in Vector4N lhs, double rhs)
    {
        return new Vector4D { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs, W = lhs.W * rhs };
    }

    static Vector4D IDivisionOperators<Vector4N, double, Vector4D>.operator /(Vector4N lhs, double rhs)
        => lhs * rhs;

    public static Vector4N operator /(int lhs, Vector4N rhs)
    {
        return new Vector4N { X = lhs / rhs.X, Y = lhs / rhs.Y, Z = lhs / rhs.Z, W = lhs / rhs.W };
    }

    public static bool operator ==(in Vector4N lhs, in Vector4N rhs)
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z && lhs.W == rhs.W;
    }

    static bool IEqualityOperators<Vector4N, Vector4N, bool>.operator ==(Vector4N lhs, Vector4N rhs)
        => lhs == rhs;

    public static bool operator !=(in Vector4N lhs, in Vector4N rhs)
    {
        return lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z || lhs.W != rhs.W;
    }

    static bool IEqualityOperators<Vector4N, Vector4N, bool>.operator !=(Vector4N lhs, Vector4N rhs)
        => lhs != rhs;
}
