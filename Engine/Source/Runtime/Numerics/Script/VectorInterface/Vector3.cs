using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
public struct Vector3 : IEquatable<Vector3>
{
    public double X;
    public double Y;
    public double Z;

    public Vector3(double scalar)
    {
        X = scalar;
        Y = scalar;
        Z = scalar;
    }

    public Vector3(double x, double y, double z)
    {
        X = x;
        Y = y;
        Z = z;
    }

    public Vector3(in Vector2 xy, double z)
    {
        X = xy.X;
        Y = xy.Y;
        Z = z;
    }

    public override string ToString() => $"({X}, {Y}, {Z})";
    public override int GetHashCode() => HashCode.Combine(X, Y, Z);
    public override bool Equals(object? obj) => obj is Vector3 other && Equals(other);

    public bool Equals(Vector3 other) => this == other;

    public bool NearlyEquals(in Vector3 other, double tolerance = 1e-8)
    {
        return Math.Abs(X - other.X) <= tolerance
            && Math.Abs(Y - other.Y) <= tolerance
            && Math.Abs(Z - other.Z) <= tolerance;
    }

    public Vector3 Swizzling(int xIndex, int yIndex, int zIndex)
    {
        var span = new ReadOnlySpan<double>(ref Unsafe.AsRef(ref X));
        return new Vector3
        (
            span[xIndex],
            span[yIndex],
            span[zIndex]
        );
    }

    public Vector2 Minor(int index) => index switch
    {
        0 => new Vector2(Y, Z),
        1 => new Vector2(X, Z),
        2 => new Vector2(X, Y),
        _ => throw new ArgumentOutOfRangeException(nameof(index), "Index must be 0, 1, or 2.")
    };

    public double this[int index]
    {
        get => index switch
        {
            0 => X,
            1 => Y,
            2 => Z,
            _ => throw new ArgumentOutOfRangeException(nameof(index), "Index must be 0, 1, or 2.")
        };
        set
        {
            switch (index)
            {
                case 0: X = value; break;
                case 1: Y = value; break;
                case 2: Z = value; break;
                default: throw new ArgumentOutOfRangeException(nameof(index), "Index must be 0, 1, or 2.");
            }
        }
    }

    public double LengthSq => X * X + Y * Y + Z * Z;

    public double Length => Math.Sqrt(LengthSq);

    public Vector3 Normalized => this / Length;

    public static double DistanceSq(in Vector3 lhs, in Vector3 rhs) => (lhs - rhs).LengthSq;
    public static double Distance(in Vector3 lhs, in Vector3 rhs) => (lhs - rhs).Length;
    public static Vector3 Normalize(in Vector3 v) => v / v.Length;
    public static double Dot(in Vector3 lhs, in Vector3 rhs) => lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
    public static Vector3 Cross(in Vector3 lhs, in Vector3 rhs) =>
        new Vector3(
            lhs.Y * rhs.Z - lhs.Z * rhs.Y,
            lhs.Z * rhs.X - lhs.X * rhs.Z,
            lhs.X * rhs.Y - lhs.Y * rhs.X
        );
    public static Vector3 Clamp(in Vector3 v, in Vector3 min, in Vector3 max) => new(
        Math.Clamp(v.X, min.X, max.X),
        Math.Clamp(v.Y, min.Y, max.Y),
        Math.Clamp(v.Z, min.Z, max.Z)
    );
    public static Vector3 Lerp(in Vector3 a, in Vector3 b, double t) => a + (b - a) * t;
    public static Vector3 Reflect(in Vector3 v, in Vector3 normal) => v - 2 * Dot(v, normal) * normal;
    public static Vector3 Project(in Vector3 v, in Vector3 onto) => Dot(v, onto) / onto.LengthSq * onto;
    public static double Angle(in Vector3 a, in Vector3 b) => Math.Acos(Dot(a, b) / (a.Length * b.Length));
    public static Vector3 Max(in Vector3 a, in Vector3 b) => new(Math.Max(a.X, b.X), Math.Max(a.Y, b.Y), Math.Max(a.Z, b.Z));
    public static Vector3 Min(in Vector3 a, in Vector3 b) => new(Math.Min(a.X, b.X), Math.Min(a.Y, b.Y), Math.Min(a.Z, b.Z));
    public static Vector3 Abs(in Vector3 v) => new(Math.Abs(v.X), Math.Abs(v.Y), Math.Abs(v.Z));
    public static bool IsZero(in Vector3 v) => v.X == 0 && v.Y == 0 && v.Z == 0;

    public static Vector3 Zero => new(0);
    public static Vector3 One => new(1);

    public static Vector3 operator +(in Vector3 lhs, in Vector3 rhs) => new(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
    public static Vector3 operator -(in Vector3 lhs, in Vector3 rhs) => new(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z);
    public static Vector3 operator *(in Vector3 lhs, in Vector3 rhs) => new(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z);
    public static Vector3 operator *(in Vector3 lhs, double rhs) => new(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs);
    public static Vector3 operator *(double lhs, in Vector3 rhs) => new(lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z);
    public static Vector3 operator /(in Vector3 lhs, in Vector3 rhs) => new(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z);
    public static Vector3 operator /(in Vector3 lhs, double rhs) => new(lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs);
    public static Vector3 operator /(double lhs, in Vector3 rhs) => new(lhs / rhs.X, lhs / rhs.Y, lhs / rhs.Z);
    public static Vector3 operator %(in Vector3 lhs, in Vector3 rhs) => new(lhs.X % rhs.X, lhs.Y % rhs.Y, lhs.Z % rhs.Z);
    public static Vector3 operator %(in Vector3 lhs, double rhs) => new(lhs.X % rhs, lhs.Y % rhs, lhs.Z % rhs);
    public static Vector3 operator %(double lhs, in Vector3 rhs) => new(lhs % rhs.X, lhs % rhs.Y, lhs % rhs.Z);

    public static bool operator ==(in Vector3 lhs, in Vector3 rhs) => lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
    public static bool operator !=(in Vector3 lhs, in Vector3 rhs) => lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z;

    public static implicit operator Vector3((double X, double Y, double Z) tuple) => new(tuple.X, tuple.Y, tuple.Z);
    public static explicit operator Vector3(double scalar) => new(scalar);
    public static explicit operator Vector2(in Vector3 v) => new(v.X, v.Y);
    public static implicit operator Vector4(in Vector3 v) => new(v.X, v.Y, v.Z, 0);
}
