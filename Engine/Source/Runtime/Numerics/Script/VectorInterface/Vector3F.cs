// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System;

namespace Ayla;

public struct Vector3F
    : IAdditionOperators<Vector3F, Vector3F, Vector3F>
    , ISubtractionOperators<Vector3F, Vector3F, Vector3F>
    , IMultiplyOperators<Vector3F, float, Vector3F>
    , IDivisionOperators<Vector3F, float, Vector3F>
    , IEqualityOperators<Vector3F, Vector3F, bool>
    , IEquatable<Vector3F>
{
    public float X;
    public float Y;
    public float Z;

    public override bool Equals([NotNullWhen(true)] object? obj)
    {
        if (obj is Vector3F v)
        {
            return Equals(v);
        }

        return false;
    }

    public bool Equals(in Vector3F rhs)
    {
        return X == rhs.X && Y == rhs.Y && Z == rhs.Z;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y, Z);
    }

    bool IEquatable<Vector3F>.Equals(Vector3F rhs) => Equals(rhs);

    public static Vector3F operator +(in Vector3F lhs, in Vector3F rhs)
    {
        return new Vector3F { X = lhs.X + rhs.X, Y = lhs.Y + rhs.Y, Z = lhs.Z + rhs.Z };
    }

    static Vector3F IAdditionOperators<Vector3F, Vector3F, Vector3F>.operator +(Vector3F lhs, Vector3F rhs)
        => lhs + rhs;

    public static Vector3F operator -(in Vector3F lhs, in Vector3F rhs)
    {
        return new Vector3F { X = lhs.X - rhs.X, Y = lhs.Y - rhs.Y, Z = lhs.Z - rhs.Z };
    }

    static Vector3F ISubtractionOperators<Vector3F, Vector3F, Vector3F>.operator -(Vector3F lhs, Vector3F rhs)
        => lhs - rhs;

    public static Vector3F operator *(in Vector3F lhs, float rhs)
    {
        return new Vector3F { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs };
    }

    static Vector3F IMultiplyOperators<Vector3F, float, Vector3F>.operator *(Vector3F lhs, float rhs)
        => lhs * rhs;

    public static Vector3F operator *(float lhs, in Vector3F rhs)
    {
        return new Vector3F { X = lhs * rhs.X, Y = lhs * rhs.Y, Z = lhs * rhs.Z };
    }

    public static Vector3F operator /(in Vector3F lhs, float rhs)
    {
        return new Vector3F { X = lhs.X / rhs, Y = lhs.Y / rhs, Z = lhs.Z / rhs };
    }

    static Vector3F IDivisionOperators<Vector3F, float, Vector3F>.operator /(Vector3F lhs, float rhs)
        => lhs / rhs;

    public static Vector3F operator /(float lhs, Vector3F rhs)
    {
        return new Vector3F { X = lhs / rhs.X, Y = lhs / rhs.Y, Z = lhs / rhs.Z };
    }

    public static bool operator ==(in Vector3F lhs, in Vector3F rhs)
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
    }

    static bool IEqualityOperators<Vector3F, Vector3F, bool>.operator ==(Vector3F lhs, Vector3F rhs)
        => lhs == rhs;

    public static bool operator !=(in Vector3F lhs, in Vector3F rhs)
    {
        return lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z;
    }

    static bool IEqualityOperators<Vector3F, Vector3F, bool>.operator !=(Vector3F lhs, Vector3F rhs)
        => lhs != rhs;
}
