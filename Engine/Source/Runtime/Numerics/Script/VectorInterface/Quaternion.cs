using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential)]
public struct Quaternion : IEquatable<Quaternion>
{
    public double X;
    public double Y;
    public double Z;
    public double W;

    public Quaternion(double x, double y, double z, double w)
    {
        X = x;
        Y = y;
        Z = z;
        W = w;
    }

    public Quaternion(in Vector3 v, double w)
    {
        X = v.X;
        Y = v.Y;
        Z = v.Z;
        W = w;
    }

    public Quaternion(in Vector4 v)
    {
        X = v.X;
        Y = v.Y;
        Z = v.Z;
        W = v.W;
    }

    public override string ToString() => $"({X}, {Y}, {Z}, {W})";
    public override int GetHashCode() => HashCode.Combine(X, Y, Z, W);
    public override bool Equals(object? obj) => obj is Quaternion other && Equals(other);

    public bool Equals(Quaternion other) => this == other;

    public bool NearlyEquals(in Quaternion other, double tolerance = 1e-8)
    {
        return Math.Abs(X - other.X) <= tolerance
            && Math.Abs(Y - other.Y) <= tolerance
            && Math.Abs(Z - other.Z) <= tolerance
            && Math.Abs(W - other.W) <= tolerance;
    }

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

    public Quaternion Normalized => this / Length;

    public static double Dot(in Quaternion a, in Quaternion b)
        => a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;

    public static Quaternion Conjugate(in Quaternion q)
        => new(-q.X, -q.Y, -q.Z, q.W);

    public Quaternion Conjugate() => Conjugate(this);

    public static Quaternion Inverse(in Quaternion q)
    {
        double lenSq = q.LengthSq;
        if (lenSq == 0) return Identity;
        var conj = Conjugate(q);
        return new(conj.X / lenSq, conj.Y / lenSq, conj.Z / lenSq, conj.W / lenSq);
    }

    public Quaternion Inverse() => Inverse(this);

    public static Quaternion operator -(in Quaternion q)
        => new(-q.X, -q.Y, -q.Z, -q.W);

    public static Quaternion operator +(in Quaternion a, in Quaternion b)
        => new(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);

    public static Quaternion operator -(in Quaternion a, in Quaternion b)
        => new(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);

    public static Quaternion operator *(in Quaternion a, double s)
        => new(a.X * s, a.Y * s, a.Z * s, a.W * s);

    public static Quaternion operator *(double s, in Quaternion a)
        => new(a.X * s, a.Y * s, a.Z * s, a.W * s);

    public static Vector3 operator *(in Quaternion q, in Vector3 v) => q.Transform(v);

    public static Quaternion operator /(in Quaternion a, double s)
        => new(a.X / s, a.Y / s, a.Z / s, a.W / s);

    public static Quaternion operator *(in Quaternion a, in Quaternion b)
    {
        // Quaternion multiplication (Hamilton product)
        return new(
            a.W * b.X + a.X * b.W + a.Y * b.Z - a.Z * b.Y,
            a.W * b.Y - a.X * b.Z + a.Y * b.W + a.Z * b.X,
            a.W * b.Z + a.X * b.Y - a.Y * b.X + a.Z * b.W,
            a.W * b.W - a.X * b.X - a.Y * b.Y - a.Z * b.Z
        );
    }

    public static bool operator ==(in Quaternion a, in Quaternion b)
        => a.X == b.X && a.Y == b.Y && a.Z == b.Z && a.W == b.W;

    public static bool operator !=(in Quaternion a, in Quaternion b)
        => a.X != b.X || a.Y != b.Y || a.Z != b.Z || a.W != b.W;

    public static explicit operator Vector4(in Quaternion q) => new(q.X, q.Y, q.Z, q.W);
    public static explicit operator Quaternion(in Vector4 v) => new(v);

    public static Quaternion Lerp(in Quaternion a, in Quaternion b, double t)
        => (a * (1.0 - t) + b * t).Normalized;

    public static Quaternion Slerp(in Quaternion a, Quaternion b, double t)
    {
        double dot = Dot(a, b);
        const double threshold = 0.9995;
        if (dot < 0.0)
        {
            b = -b;
            dot = -dot;
        }
        if (dot > threshold)
        {
            return Lerp(a, b, t);
        }
        dot = Math.Clamp(dot, -1.0, 1.0);
        double theta_0 = Math.Acos(dot);
        double theta = theta_0 * t;
        double sin_theta_0 = Math.Sin(theta_0);
        double sin_theta = Math.Sin(theta);
        double s0 = Math.Cos(theta) - dot * sin_theta / sin_theta_0;
        double s1 = sin_theta / sin_theta_0;
        return (a * s0 + b * s1).Normalized;
    }

    public static Quaternion FromAxisAngle(in Vector3 axis, double angleRad)
    {
        var norm = Vector3.Normalize(axis);
        double half = angleRad * 0.5;
        double s = Math.Sin(half);
        double c = Math.Cos(half);
        return new(norm.X * s, norm.Y * s, norm.Z * s, c);
    }

    public static Quaternion Max(in Quaternion a, in Quaternion b)
        => new(Math.Max(a.X, b.X), Math.Max(a.Y, b.Y), Math.Max(a.Z, b.Z), Math.Max(a.W, b.W));

    public static Quaternion Min(in Quaternion a, in Quaternion b)
        => new(Math.Min(a.X, b.X), Math.Min(a.Y, b.Y), Math.Min(a.Z, b.Z), Math.Min(a.W, b.W));

    public static Quaternion Abs(in Quaternion q)
        => new(Math.Abs(q.X), Math.Abs(q.Y), Math.Abs(q.Z), Math.Abs(q.W));

    public static bool IsZero(in Quaternion q)
        => q.X == 0 && q.Y == 0 && q.Z == 0 && q.W == 0;

    public static Quaternion Identity => new(0, 0, 0, 1);

    public Vector3 Transform(in Vector3 v)
    {
        // q * v * q^-1
        var qv = new Quaternion(v, 0);
        var result = this * qv * this.Inverse();
        return new Vector3(result.X, result.Y, result.Z);
    }
}
