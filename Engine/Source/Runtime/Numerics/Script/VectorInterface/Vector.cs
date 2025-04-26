// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System;

namespace Ayla;

public static class Vector
{
    public static float LengthSq(in this Vector2F self)
    {
        return self.X * self.X + self.Y * self.Y;
    }

    public static double LengthSq(in this Vector2D self)
    {
        return self.X * self.X + self.Y * self.Y;
    }

    public static float Length(in this Vector2F self)
    {
        return (float)Math.Sqrt(self.LengthSq());
    }

    public static double Length(in this Vector2D self)
    {
        return Math.Sqrt(self.LengthSq());
    }

    public static float DistanceSq(in this Vector2F self, in Vector2F target)
    {
        return (target - self).LengthSq();
    }

    public static double DistanceSq(in this Vector2D self, in Vector2D target)
    {
        return (target - self).LengthSq();
    }
}
