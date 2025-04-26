// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System;

namespace Ayla;

public struct Vector4F
    : IAdditionOperators<Vector4F, Vector4F, Vector4F>
    , ISubtractionOperators<Vector4F, Vector4F, Vector4F>
    , IMultiplyOperators<Vector4F, float, Vector4F>
    , IDivisionOperators<Vector4F, float, Vector4F>
    , IEqualityOperators<Vector4F, Vector4F, bool>
    , IEquatable<Vector4F>
{
    public float X;
    public float Y;
    public float Z;
    public float W;

    public override bool Equals([NotNullWhen(true)] object? obj)
    {
        if (obj is Vector4F v)
        {
            return Equals(v);
        }

        return false;
    }

    public bool Equals(in Vector4F rhs)
    {
        return X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y, Z, W);
    }

    bool IEquatable<Vector4F>.Equals(Vector4F rhs) => Equals(rhs);

    public static Vector4F operator +(in Vector4F lhs, in Vector4F rhs)
    {
        return new Vector4F { X = lhs.X + rhs.X, Y = lhs.Y + rhs.Y, Z = lhs.Z + rhs.Z, W = lhs.W + rhs.W };
    }

    static Vector4F IAdditionOperators<Vector4F, Vector4F, Vector4F>.operator +(Vector4F lhs, Vector4F rhs)
        => lhs + rhs;

    public static Vector4F operator -(in Vector4F lhs, in Vector4F rhs)
    {
        return new Vector4F { X = lhs.X - rhs.X, Y = lhs.Y - rhs.Y, Z = lhs.Z - rhs.Z, W = lhs.W - rhs.W };
    }

    static Vector4F ISubtractionOperators<Vector4F, Vector4F, Vector4F>.operator -(Vector4F lhs, Vector4F rhs)
        => lhs - rhs;

    public static Vector4F operator *(in Vector4F lhs, float rhs)
    {
        return new Vector4F { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs, W = lhs.W * rhs };
    }

    static Vector4F IMultiplyOperators<Vector4F, float, Vector4F>.operator *(Vector4F lhs, float rhs)
        => lhs * rhs;

    public static Vector4F operator *(float lhs, in Vector4F rhs)
    {
        return new Vector4F { X = lhs * rhs.X, Y = lhs * rhs.Y, Z = lhs * rhs.Z, W = lhs * rhs.W };
    }

    public static Vector4F operator /(in Vector4F lhs, float rhs)
    {
        return new Vector4F { X = lhs.X / rhs, Y = lhs.Y / rhs, Z = lhs.Z / rhs, W = lhs.W / rhs };
    }

    static Vector4F IDivisionOperators<Vector4F, float, Vector4F>.operator /(Vector4F lhs, float rhs)
        => lhs / rhs;

    public static Vector4F operator /(float lhs, Vector4F rhs)
    {
        return new Vector4F { X = lhs / rhs.X, Y = lhs / rhs.Y, Z = lhs / rhs.Z, W = lhs / rhs.W };
    }

    public static bool operator ==(in Vector4F lhs, in Vector4F rhs)
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z && lhs.W == rhs.W;
    }

    static bool IEqualityOperators<Vector4F, Vector4F, bool>.operator ==(Vector4F lhs, Vector4F rhs)
        => lhs == rhs;

    public static bool operator !=(in Vector4F lhs, in Vector4F rhs)
    {
        return lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z || lhs.W != rhs.W;
    }

    static bool IEqualityOperators<Vector4F, Vector4F, bool>.operator !=(Vector4F lhs, Vector4F rhs)
        => lhs != rhs;
}
