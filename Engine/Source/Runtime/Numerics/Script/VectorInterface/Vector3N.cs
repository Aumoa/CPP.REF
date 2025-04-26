// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;
using System.Diagnostics.CodeAnalysis;
using System.Numerics;

namespace Ayla;

public struct Vector3N
    : IAdditionOperators<Vector3N, Vector3N, Vector3N>
    , ISubtractionOperators<Vector3N, Vector3N, Vector3N>
    , IMultiplyOperators<Vector3N, int, Vector3N>
    , IMultiplyOperators<Vector3N, float, Vector3F>
    , IMultiplyOperators<Vector3N, double, Vector3D>
    , IDivisionOperators<Vector3N, int, Vector3N>
    , IDivisionOperators<Vector3N, float, Vector3F>
    , IDivisionOperators<Vector3N, double, Vector3D>
    , IEqualityOperators<Vector3N, Vector3N, bool>
    , IEquatable<Vector3N>
{
    public int X;
    public int Y;
    public int Z;

    public override bool Equals([NotNullWhen(true)] object? obj)
    {
        if (obj is Vector3N v)
        {
            return Equals(v);
        }

        return false;
    }

    public bool Equals(in Vector3N rhs)
    {
        return X == rhs.X && Y == rhs.Y && Z == rhs.Z;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y, Z);
    }

    bool IEquatable<Vector3N>.Equals(Vector3N rhs) => Equals(rhs);

    public static Vector3N operator +(in Vector3N lhs, in Vector3N rhs)
    {
        return new Vector3N { X = lhs.X + rhs.X, Y = lhs.Y + rhs.Y, Z = lhs.Z + rhs.Z };
    }

    static Vector3N IAdditionOperators<Vector3N, Vector3N, Vector3N>.operator +(Vector3N lhs, Vector3N rhs)
        => lhs + rhs;

    public static Vector3N operator -(in Vector3N lhs, in Vector3N rhs)
    {
        return new Vector3N { X = lhs.X - rhs.X, Y = lhs.Y - rhs.Y, Z = lhs.Z - rhs.Z };
    }

    static Vector3N ISubtractionOperators<Vector3N, Vector3N, Vector3N>.operator -(Vector3N lhs, Vector3N rhs)
        => lhs - rhs;

    public static Vector3N operator *(in Vector3N lhs, int rhs)
    {
        return new Vector3N { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs };
    }

    static Vector3N IMultiplyOperators<Vector3N, int, Vector3N>.operator *(Vector3N lhs, int rhs)
        => lhs * rhs;

    public static Vector3F operator *(in Vector3N lhs, float rhs)
    {
        return new Vector3F { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs };
    }

    static Vector3F IMultiplyOperators<Vector3N, float, Vector3F>.operator *(Vector3N lhs, float rhs)
        => lhs * rhs;

    public static Vector3D operator *(in Vector3N lhs, double rhs)
    {
        return new Vector3D { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs };
    }

    static Vector3D IMultiplyOperators<Vector3N, double, Vector3D>.operator *(Vector3N lhs, double rhs)
        => lhs * rhs;

    public static Vector3N operator *(int lhs, in Vector3N rhs)
    {
        return new Vector3N { X = lhs * rhs.X, Y = lhs * rhs.Y, Z = lhs * rhs.Z };
    }

    public static Vector3N operator /(in Vector3N lhs, int rhs)
    {
        return new Vector3N { X = lhs.X / rhs, Y = lhs.Y / rhs, Z = lhs.Z / rhs };
    }

    static Vector3N IDivisionOperators<Vector3N, int, Vector3N>.operator /(Vector3N lhs, int rhs)
        => lhs / rhs;

    public static Vector3F operator /(in Vector3N lhs, float rhs)
    {
        return new Vector3F { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs };
    }

    static Vector3F IDivisionOperators<Vector3N, float, Vector3F>.operator /(Vector3N lhs, float rhs)
        => lhs * rhs;

    public static Vector3D operator /(in Vector3N lhs, double rhs)
    {
        return new Vector3D { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs };
    }

    static Vector3D IDivisionOperators<Vector3N, double, Vector3D>.operator /(Vector3N lhs, double rhs)
        => lhs * rhs;

    public static Vector3N operator /(int lhs, Vector3N rhs)
    {
        return new Vector3N { X = lhs / rhs.X, Y = lhs / rhs.Y, Z = lhs / rhs.Z };
    }

    public static bool operator ==(in Vector3N lhs, in Vector3N rhs)
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
    }

    static bool IEqualityOperators<Vector3N, Vector3N, bool>.operator ==(Vector3N lhs, Vector3N rhs)
        => lhs == rhs;

    public static bool operator !=(in Vector3N lhs, in Vector3N rhs)
    {
        return lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z;
    }

    static bool IEqualityOperators<Vector3N, Vector3N, bool>.operator !=(Vector3N lhs, Vector3N rhs)
        => lhs != rhs;
}
