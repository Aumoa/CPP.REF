// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct SlateRenderTransform;
struct SlateLayoutTransform;
struct SlateRotatedRect;
struct Margin;

class TransformCalculus2D
{
public:
	template<class T>
	static constexpr Vector2 TransformPoint(const T& Transform, const Vector2& Point)
	{
		return Transform.TransformPoint(Point);
	}

	static constexpr Vector2 TransformPoint(const Vector2& Transform, const Vector2& Point)
	{
		return Transform + Point;
	}

	static constexpr Vector2 TransformPoint(float Transform, const Vector2& Point)
	{
		return Transform * Point;
	}

	template<class T>
	static constexpr Vector2 TransformVector(const T& Transform, const Vector2& Vector)
	{
		return Transform.TransformVector(Vector);
	}

	static constexpr Vector2 TransformVector(const Vector2& Transform, const Vector2& Vector)
	{
		return Vector;
	}

	static constexpr Vector2 TransformVector(float Transform, const Vector2& Vector)
	{
		return Transform * Vector;
	}

	template<class T>
	static constexpr Rect TransformRect(const T& Transform, const Rect& InRect)
	{
		const Vector2 TL = TransformPoint(Transform, Vector2(InRect.Left, InRect.Top));
		const Vector2 RB = TransformPoint(Transform, Vector2(InRect.Right, InRect.Bottom));
		return Rect(MathEx::Min(TL.X, RB.X), MathEx::Min(TL.Y, RB.Y), MathEx::Max(TL.X, RB.X), MathEx::Max(TL.Y, RB.Y));
	}

	// LayoutImpl.h
	template<class T>
	static constexpr SlateRotatedRect TransformRect(const T& Transform, const SlateRotatedRect& Rect);

	template<class T>
	static constexpr auto Inverse(const T& Transform)
	{
		return Transform.Inverse();
	}

	static constexpr Vector2 Inverse(const Vector2& Transform)
	{
		return -Transform;
	}

	template<class TFirst, class TSecond, class... TTransformTypeArgs>
	static constexpr auto Concatenate(const TFirst& First, const TSecond& Second, const TTransformTypeArgs&... Args)
	{
		if constexpr (sizeof...(Args) == 0)
		{
			return Concatenate(First, Second);
		}
		else
		{
			return Concatenate(Concatenate(First, Second), Args...);
		}
	}

	template<class TFirst, class TSecond>
	static constexpr auto Concatenate(const TFirst& First, const TSecond& Second)
	{
		return First.Concatenate(Second);
	}

	static constexpr Vector2 Concatenate(const Vector2& Translation, const Vector2& Point)
	{
		return Translation + Point;
	}

	static constexpr float Concatenate(float Transform, float Scalar)
	{
		return Transform * Scalar;
	}

	// LayoutImpl.h
	static constexpr SlateRenderTransform Concatenate(const Vector2& Translation, const SlateRenderTransform& Transform);
	static constexpr SlateRenderTransform Concatenate(const SlateRenderTransform& Transform, const Vector2& Translation);
	static constexpr SlateRenderTransform Concatenate(const SlateRenderTransform& Transform, const SlateLayoutTransform& Layout);
	static constexpr SlateRenderTransform Concatenate(const SlateLayoutTransform& Layout, const SlateRenderTransform& Transform);

	static constexpr bool IsIntersect(const Rect& Lhs, const Rect& Rhs)
	{
		const float Left = MathEx::Max(Lhs.Left, Rhs.Left);
		const float Top = MathEx::Max(Lhs.Top, Rhs.Top);
		const float Right = MathEx::Min(Lhs.Right, Rhs.Right);
		const float Bottom = MathEx::Min(Lhs.Bottom, Rhs.Bottom);

		if (Right >= Left && Bottom >= Top)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool IsIntersect(const Rect& Lhs, const Rect& Rhs, Rect& Overlap)
	{
		float Left = MathEx::Max(Lhs.Left, Rhs.Left);
		float Top = MathEx::Max(Lhs.Top, Rhs.Top);
		float Right = MathEx::Min(Lhs.Right, Rhs.Right);
		float Bottom = MathEx::Min(Lhs.Bottom, Rhs.Bottom);

		if (Right >= Left && Bottom >= Top)
		{
			Overlap = Rect(Left, Top, Right, Bottom);
			return true;
		}
		else
		{
			Overlap = Rect();
			return false;
		}
	}

	// LayoutImpl.h
	static constexpr Rect ExtendBy(const Rect& Lhs, const Margin& Rhs);
};