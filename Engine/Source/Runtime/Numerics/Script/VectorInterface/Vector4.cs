using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
public struct Vector4 : IEquatable<Vector4>
{
    public double X;
    public double Y;
    public double Z;
    public double W;

    public Vector4(double scalar)
    {
        X = scalar;
        Y = scalar;
        Z = scalar;
        W = scalar;
    }

    public Vector4(double x, double y, double z, double w)
    {
        X = x;
        Y = y;
        Z = z;
        W = w;
    }

    public Vector4(in Vector3 xyz, double w)
    {
        X = xyz.X;
        Y = xyz.Y;
        Z = xyz.Z;
        W = w;
    }

    public override string ToString() => $"({X}, {Y}, {Z}, {W})";
    public override int GetHashCode() => HashCode.Combine(X, Y, Z, W);
    public override bool Equals(object? obj) => obj is Vector4 other && Equals(other);

    public bool Equals(Vector4 other) => this == other;

    public bool NearlyEquals(in Vector4 other, double tolerance = 1e-8)
    {
        return Math.Abs(X - other.X) <= tolerance
            && Math.Abs(Y - other.Y) <= tolerance
            && Math.Abs(Z - other.Z) <= tolerance
            && Math.Abs(W - other.W) <= tolerance;
    }

    public Vector4 Swizzling(int xIndex, int yIndex, int zIndex, int wIndex)
    {
        var span = new ReadOnlySpan<double>(ref Unsafe.AsRef(ref X));
        return new Vector4(
            span[xIndex],
            span[yIndex],
            span[zIndex],
            span[wIndex]
        );
    }

    public Vector3 Minor(int index) => index switch
    {
        0 => new Vector3(Y, Z, W),
        1 => new Vector3(X, Z, W),
        2 => new Vector3(X, Y, W),
        3 => new Vector3(X, Y, Z),
        _ => throw new ArgumentOutOfRangeException(nameof(index), "Index must be 0, 1, 2, or 3.")
    };

    public double this[int index]
    {
        get => index switch
        {
            0 => X,
            1 => Y,
            2 => Z,
            3 => W,
            _ => throw new ArgumentOutOfRangeException(nameof(index), "Index must be 0, 1, 2, or 3.")
        };
        set
        {
            switch (index)
            {
                case 0: X = value; break;
                case 1: Y = value; break;
                case 2: Z = value; break;
                case 3: W = value; break;
                default: throw new ArgumentOutOfRangeException(nameof(index), "Index must be 0, 1, 2, or 3.");
            }
        }
    }

    public double LengthSq => X * X + Y * Y + Z * Z + W * W;

    public double Length => Math.Sqrt(LengthSq);

    public Vector4 Normalized => this / Length;

    public static double DistanceSq(in Vector4 lhs, in Vector4 rhs) => (lhs - rhs).LengthSq;
    public static double Distance(in Vector4 lhs, in Vector4 rhs) => (lhs - rhs).Length;
    public static Vector4 Normalize(in Vector4 v) => v / v.Length;
    public static double Dot(in Vector4 lhs, in Vector4 rhs) => lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z + lhs.W * rhs.W;
    public static Vector4 Clamp(in Vector4 v, in Vector4 min, in Vector4 max) => new(
        Math.Clamp(v.X, min.X, max.X),
        Math.Clamp(v.Y, min.Y, max.Y),
        Math.Clamp(v.Z, min.Z, max.Z),
        Math.Clamp(v.W, min.W, max.W)
    );
    public static Vector4 Lerp(in Vector4 a, in Vector4 b, double t) => a + (b - a) * t;
    public static Vector4 Max(in Vector4 a, in Vector4 b) => new(Math.Max(a.X, b.X), Math.Max(a.Y, b.Y), Math.Max(a.Z, b.Z), Math.Max(a.W, b.W));
    public static Vector4 Min(in Vector4 a, in Vector4 b) => new(Math.Min(a.X, b.X), Math.Min(a.Y, b.Y), Math.Min(a.Z, b.Z), Math.Min(a.W, b.W));
    public static Vector4 Abs(in Vector4 v) => new(Math.Abs(v.X), Math.Abs(v.Y), Math.Abs(v.Z), Math.Abs(v.W));
    public static bool IsZero(in Vector4 v) => v.X == 0 && v.Y == 0 && v.Z == 0 && v.W == 0;

    public static Vector4 Zero => new(0);
    public static Vector4 One => new(1);

    public static Vector4 operator +(in Vector4 lhs, in Vector4 rhs) => new(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W);
    public static Vector4 operator -(in Vector4 lhs, in Vector4 rhs) => new(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W);
    public static Vector4 operator *(in Vector4 lhs, in Vector4 rhs) => new(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z, lhs.W * rhs.W);
    public static Vector4 operator *(in Vector4 lhs, double rhs) => new(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs, lhs.W * rhs);
    public static Vector4 operator *(double lhs, in Vector4 rhs) => new(lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z, lhs * rhs.W);
    public static Vector4 operator /(in Vector4 lhs, in Vector4 rhs) => new(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z, lhs.W / rhs.W);
    public static Vector4 operator /(in Vector4 lhs, double rhs) => new(lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs, lhs.W / rhs);
    public static Vector4 operator /(double lhs, in Vector4 rhs) => new(lhs / rhs.X, lhs / rhs.Y, lhs / rhs.Z, lhs / rhs.W);
    public static Vector4 operator %(in Vector4 lhs, in Vector4 rhs) => new(lhs.X % rhs.X, lhs.Y % rhs.Y, lhs.Z % rhs.Z, lhs.W % rhs.W);
    public static Vector4 operator %(in Vector4 lhs, double rhs) => new(lhs.X % rhs, lhs.Y % rhs, lhs.Z % rhs, lhs.W % rhs);
    public static Vector4 operator %(double lhs, in Vector4 rhs) => new(lhs % rhs.X, lhs % rhs.Y, lhs % rhs.Z, lhs % rhs.W);

    public static bool operator ==(in Vector4 lhs, in Vector4 rhs) => lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z && lhs.W == rhs.W;
    public static bool operator !=(in Vector4 lhs, in Vector4 rhs) => lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z || lhs.W != rhs.W;

    public static implicit operator Vector4((double X, double Y, double Z, double W) tuple) => new(tuple.X, tuple.Y, tuple.Z, tuple.W);
    public static explicit operator Vector4(double scalar) => new(scalar);
    public static explicit operator Vector3(in Vector4 v) => new(v.X, v.Y, v.Z);
}
