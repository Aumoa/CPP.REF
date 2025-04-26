// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System;

namespace Ayla;

public struct Vector3D
    : IAdditionOperators<Vector3D, Vector3D, Vector3D>
    , ISubtractionOperators<Vector3D, Vector3D, Vector3D>
    , IMultiplyOperators<Vector3D, double, Vector3D>
    , IDivisionOperators<Vector3D, double, Vector3D>
    , IEqualityOperators<Vector3D, Vector3D, bool>
    , IEquatable<Vector3D>
{
    public double X;
    public double Y;
    public double Z;

    public override bool Equals([NotNullWhen(true)] object? obj)
    {
        if (obj is Vector3D v)
        {
            return Equals(v);
        }

        return false;
    }

    public bool Equals(in Vector3D rhs)
    {
        return X == rhs.X && Y == rhs.Y && Z == rhs.Z;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y, Z);
    }

    bool IEquatable<Vector3D>.Equals(Vector3D rhs) => Equals(rhs);

    public static Vector3D operator +(in Vector3D lhs, in Vector3D rhs)
    {
        return new Vector3D { X = lhs.X + rhs.X, Y = lhs.Y + rhs.Y, Z = lhs.Z + rhs.Z };
    }

    static Vector3D IAdditionOperators<Vector3D, Vector3D, Vector3D>.operator +(Vector3D lhs, Vector3D rhs)
        => lhs + rhs;

    public static Vector3D operator -(in Vector3D lhs, in Vector3D rhs)
    {
        return new Vector3D { X = lhs.X - rhs.X, Y = lhs.Y - rhs.Y, Z = lhs.Z - rhs.Z };
    }

    static Vector3D ISubtractionOperators<Vector3D, Vector3D, Vector3D>.operator -(Vector3D lhs, Vector3D rhs)
        => lhs - rhs;

    public static Vector3D operator *(in Vector3D lhs, double rhs)
    {
        return new Vector3D { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs };
    }

    static Vector3D IMultiplyOperators<Vector3D, double, Vector3D>.operator *(Vector3D lhs, double rhs)
        => lhs * rhs;

    public static Vector3D operator *(double lhs, in Vector3D rhs)
    {
        return new Vector3D { X = lhs * rhs.X, Y = lhs * rhs.Y, Z = lhs * rhs.Z };
    }

    public static Vector3D operator /(in Vector3D lhs, double rhs)
    {
        return new Vector3D { X = lhs.X / rhs, Y = lhs.Y / rhs, Z = lhs.Z / rhs };
    }

    static Vector3D IDivisionOperators<Vector3D, double, Vector3D>.operator /(Vector3D lhs, double rhs)
        => lhs / rhs;

    public static Vector3D operator /(double lhs, Vector3D rhs)
    {
        return new Vector3D { X = lhs / rhs.X, Y = lhs / rhs.Y, Z = lhs / rhs.Z };
    }

    public static bool operator ==(in Vector3D lhs, in Vector3D rhs)
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
    }

    static bool IEqualityOperators<Vector3D, Vector3D, bool>.operator ==(Vector3D lhs, Vector3D rhs)
        => lhs == rhs;

    public static bool operator !=(in Vector3D lhs, in Vector3D rhs)
    {
        return lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z;
    }

    static bool IEqualityOperators<Vector3D, Vector3D, bool>.operator !=(Vector3D lhs, Vector3D rhs)
        => lhs != rhs;
}
