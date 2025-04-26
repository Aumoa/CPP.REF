// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
public struct Vector4D
    : IAdditionOperators<Vector4D, Vector4D, Vector4D>
    , ISubtractionOperators<Vector4D, Vector4D, Vector4D>
    , IMultiplyOperators<Vector4D, double, Vector4D>
    , IDivisionOperators<Vector4D, double, Vector4D>
    , IEqualityOperators<Vector4D, Vector4D, bool>
    , IEquatable<Vector4D>
{
    public double X;
    public double Y;
    public double Z;
    public double W;

    public override bool Equals([NotNullWhen(true)] object? obj)
    {
        if (obj is Vector4D v)
        {
            return Equals(v);
        }

        return false;
    }

    public bool Equals(in Vector4D rhs)
    {
        return X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W;
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(X, Y, Z, W);
    }

    bool IEquatable<Vector4D>.Equals(Vector4D rhs) => Equals(rhs);

    public static Vector4D operator +(in Vector4D lhs, in Vector4D rhs)
    {
        return new Vector4D { X = lhs.X + rhs.X, Y = lhs.Y + rhs.Y, Z = lhs.Z + rhs.Z, W = lhs.W + rhs.W };
    }

    static Vector4D IAdditionOperators<Vector4D, Vector4D, Vector4D>.operator +(Vector4D lhs, Vector4D rhs)
        => lhs + rhs;

    public static Vector4D operator -(in Vector4D lhs, in Vector4D rhs)
    {
        return new Vector4D { X = lhs.X - rhs.X, Y = lhs.Y - rhs.Y, Z = lhs.Z - rhs.Z, W = lhs.W - rhs.W };
    }

    static Vector4D ISubtractionOperators<Vector4D, Vector4D, Vector4D>.operator -(Vector4D lhs, Vector4D rhs)
        => lhs - rhs;

    public static Vector4D operator *(in Vector4D lhs, double rhs)
    {
        return new Vector4D { X = lhs.X * rhs, Y = lhs.Y * rhs, Z = lhs.Z * rhs, W = lhs.W * rhs };
    }

    static Vector4D IMultiplyOperators<Vector4D, double, Vector4D>.operator *(Vector4D lhs, double rhs)
        => lhs * rhs;

    public static Vector4D operator *(double lhs, in Vector4D rhs)
    {
        return new Vector4D { X = lhs * rhs.X, Y = lhs * rhs.Y, Z = lhs * rhs.Z, W = lhs * rhs.W };
    }

    public static Vector4D operator /(in Vector4D lhs, double rhs)
    {
        return new Vector4D { X = lhs.X / rhs, Y = lhs.Y / rhs, Z = lhs.Z / rhs, W = lhs.W / rhs };
    }

    static Vector4D IDivisionOperators<Vector4D, double, Vector4D>.operator /(Vector4D lhs, double rhs)
        => lhs / rhs;

    public static Vector4D operator /(double lhs, Vector4D rhs)
    {
        return new Vector4D { X = lhs / rhs.X, Y = lhs / rhs.Y, Z = lhs / rhs.Z, W = lhs / rhs.W };
    }

    public static bool operator ==(in Vector4D lhs, in Vector4D rhs)
    {
        return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z && lhs.W == rhs.W;
    }

    static bool IEqualityOperators<Vector4D, Vector4D, bool>.operator ==(Vector4D lhs, Vector4D rhs)
        => lhs == rhs;

    public static bool operator !=(in Vector4D lhs, in Vector4D rhs)
    {
        return lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z || lhs.W != rhs.W;
    }

    static bool IEqualityOperators<Vector4D, Vector4D, bool>.operator !=(Vector4D lhs, Vector4D rhs)
        => lhs != rhs;
}
