using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
public struct Vector2 : IEquatable<Vector2>
{
    public double X;
    public double Y;

    public Vector2(double scalar)
    {
        X = scalar;
        Y = scalar;
    }

    public Vector2(double x, double y)
    {
        X = x;
        Y = y;
    }

    public override string ToString() => $"({X}, {Y})";
    public override int GetHashCode() => HashCode.Combine(X, Y);
    public override bool Equals(object? obj) => obj is Vector2 other && Equals(other);

    public bool Equals(Vector2 other) => this == other;

    public bool NearlyEquals(in Vector2 other, double tolerance = 1e-8)
    {
        return Math.Abs(X - other.X) <= tolerance && Math.Abs(Y - other.Y) <= tolerance;
    }

    public Vector2 Swizzling(int xIndex, int yIndex)
    {
        var span = new ReadOnlySpan<double>(ref Unsafe.AsRef(ref X));
        return new Vector2
        (
            span[xIndex],
            span[yIndex]
        );
    }

    public double this[int index]
    {
        get => index switch
        {
            0 => X,
            1 => Y,
            _ => throw new ArgumentOutOfRangeException(nameof(index), "Index must be 0 or 1.")
        };
        set
        {
            switch (index)
            {
                case 0:
                    X = value;
                    break;
                case 1:
                    Y = value;
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(index), "Index must be 0 or 1.");
            }
        }
    }

    public double LengthSq => X * X + Y * Y;

    public double Length => Math.Sqrt(LengthSq);

    public Vector2 Normalized => this / Length;

    public static double DistanceSq(in Vector2 lhs, in Vector2 rhs) => (lhs - rhs).LengthSq;
    public static double Distance(in Vector2 lhs, in Vector2 rhs) => (lhs - rhs).Length;
    public static Vector2 Normalize(in Vector2 v) => v / v.Length;
    public static double Cross(in Vector2 lhs, in Vector2 rhs) => lhs.X * rhs.Y - lhs.Y * rhs.X;
    public static double Dot(in Vector2 lhs, in Vector2 rhs) => lhs.X * rhs.X + lhs.Y * rhs.Y;
    public static Vector2 Clamp(in Vector2 v, in Vector2 min, in Vector2 max) => new(
        Math.Clamp(v.X, min.X, max.X),
        Math.Clamp(v.Y, min.Y, max.Y)
    );
    public static Vector2 Lerp(in Vector2 a, in Vector2 b, double t) => a + (b - a) * t;
    public static double Angle(in Vector2 a, in Vector2 b)
    {
        double dot = Dot(a, b);
        double len = a.Length * b.Length;
        if (len == 0) return 0;
        double cos = Math.Clamp(dot / len, -1.0, 1.0);
        return Math.Acos(cos);
    }
    public static Vector2 Max(in Vector2 a, in Vector2 b) => new(Math.Max(a.X, b.X), Math.Max(a.Y, b.Y));
    public static Vector2 Min(in Vector2 a, in Vector2 b) => new(Math.Min(a.X, b.X), Math.Min(a.Y, b.Y));
    public static Vector2 Abs(in Vector2 v) => new(Math.Abs(v.X), Math.Abs(v.Y));
    public static bool IsZero(in Vector2 v) => v.X == 0 && v.Y == 0;

    public static Vector2 Zero => new(0);
    public static Vector2 One => new(1);

    public static Vector2 operator +(in Vector2 lhs, in Vector2 rhs) => new(lhs.X + rhs.X, lhs.Y + rhs.Y);
    public static Vector2 operator -(in Vector2 lhs, in Vector2 rhs) => new(lhs.X - rhs.X, lhs.Y - rhs.Y);
    public static Vector2 operator *(in Vector2 lhs, in Vector2 rhs) => new(lhs.X * rhs.X, lhs.Y * rhs.Y);
    public static Vector2 operator *(in Vector2 lhs, double rhs) => new(lhs.X * rhs, lhs.Y * rhs);
    public static Vector2 operator *(double lhs, in Vector2 rhs) => new(lhs * rhs.X, lhs * rhs.Y);
    public static Vector2 operator /(in Vector2 lhs, in Vector2 rhs) => new(lhs.X / rhs.X, lhs.Y / rhs.Y);
    public static Vector2 operator /(in Vector2 lhs, double rhs) => new(lhs.X / rhs, lhs.Y / rhs);
    public static Vector2 operator /(double lhs, in Vector2 rhs) => new(lhs / rhs.X, lhs / rhs.Y);
    public static Vector2 operator %(in Vector2 lhs, in Vector2 rhs) => new(lhs.X % rhs.X, lhs.Y % rhs.Y);
    public static Vector2 operator %(in Vector2 lhs, double rhs) => new(lhs.X % rhs, lhs.Y % rhs);
    public static Vector2 operator %(double lhs, in Vector2 rhs) => new(lhs % rhs.X, lhs % rhs.Y);

    public static bool operator ==(in Vector2 lhs, in Vector2 rhs) => lhs.X == rhs.X && lhs.Y == rhs.Y;
    public static bool operator !=(in Vector2 lhs, in Vector2 rhs) => lhs.X != rhs.X || lhs.Y != rhs.Y;

    public static implicit operator Vector2((double X, double Y) tuple) => new(tuple.X, tuple.Y);
    public static explicit operator Vector2(double scalar) => new(scalar);
    public static implicit operator Vector3(in Vector2 v) => new(v.X, v.Y, 0);
}
