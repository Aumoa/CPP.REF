// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Concepts/LayoutTransform.h"

struct SlateRenderTransform;
struct SlateLayoutTransform;
struct SlateRotatedRect;
struct Margin;

class TransformCalculus2D
{
public:
	template<LayoutTransform T>
	static constexpr Vector2 TransformPoint(const T& transform, const Vector2& point)
	{
		return transform.TransformPoint(point);
	}

	static constexpr Vector2 TransformPoint(const Vector2& transform, const Vector2& point)
	{
		return transform + point;
	}

	static constexpr Vector2 TransformPoint(float transform, const Vector2& point)
	{
		return transform * point;
	}

	template<LayoutTransform T>
	static constexpr Vector2 TransformVector(const T& transform, const Vector2& vector)
	{
		return transform.TransformVector(vector);
	}

	static constexpr Vector2 TransformVector(const Vector2& transform, const Vector2& vector)
	{
		return vector;
	}

	static constexpr Vector2 TransformVector(float transform, const Vector2& vector)
	{
		return transform * vector;
	}

	template<LayoutTransform T>
	static constexpr Rect TransformRect(const T& transform, const Rect& rect)
	{
		const Vector2 TL = TransformPoint(transform, Vector2(rect.Left, rect.Top));
		const Vector2 RB = TransformPoint(transform, Vector2(rect.Right, rect.Bottom));
		return Rect(MathEx::Min(TL.X, RB.X), MathEx::Min(TL.Y, RB.Y), MathEx::Max(TL.X, RB.X), MathEx::Max(TL.Y, RB.Y));
	}

	// LayoutImpl.h
	template<LayoutTransform T>
	static constexpr SlateRotatedRect TransformRect(const T& transform, const SlateRotatedRect& rect);

	template<LayoutTransform T>
	static constexpr auto Inverse(const T& transform)
	{
		return transform.Inverse();
	}

	static constexpr Vector2 Inverse(const Vector2& transform)
	{
		return -transform;
	}

	template<class TFirst, class TSecond, class... TTransformTypeArgs>
	static constexpr auto Concatenate(const TFirst& first, const TSecond& second, const TTransformTypeArgs&... args)
	{
		if constexpr (sizeof...(args) == 0)
		{
			return Concatenate(first, second);
		}
		else
		{
			return Concatenate(Concatenate(first, second), args...);
		}
	}

	template<LayoutTransform TFirst, class TSecond>
	static constexpr auto Concatenate(const TFirst& first, const TSecond& second)
	{
		return first.Concatenate(second);
	}

	static constexpr Vector2 Concatenate(const Vector2& translation, const Vector2& point)
	{
		return translation + point;
	}

	static constexpr float Concatenate(float transform, float scalar)
	{
		return transform * scalar;
	}

	// LayoutImpl.h
	static constexpr SlateRenderTransform Concatenate(const Vector2& translation, const SlateRenderTransform& transform);
	static constexpr SlateRenderTransform Concatenate(const SlateRenderTransform& transform, const Vector2& translation);
	static constexpr SlateRenderTransform Concatenate(const SlateRenderTransform& transform, const SlateLayoutTransform& layout);
	static constexpr SlateRenderTransform Concatenate(const SlateLayoutTransform& layout, const SlateRenderTransform& transform);

	static constexpr bool IsIntersect(const Rect& lhs, const Rect& rhs)
	{
		const float left = MathEx::Max(lhs.Left, rhs.Left);
		const float top = MathEx::Max(lhs.Top, rhs.Top);
		const float right = MathEx::Min(lhs.Right, rhs.Right);
		const float bottom = MathEx::Min(lhs.Bottom, rhs.Bottom);

		if (right >= left && bottom >= top)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool IsIntersect(const Rect& lhs, const Rect& rhs, Rect& overlap)
	{
		float left = MathEx::Max(lhs.Left, rhs.Left);
		float top = MathEx::Max(lhs.Top, rhs.Top);
		float right = MathEx::Min(lhs.Right, rhs.Right);
		float bottom = MathEx::Min(lhs.Bottom, rhs.Bottom);

		if (right >= left && bottom >= top)
		{
			overlap = Rect(left, top, right, bottom);
			return true;
		}
		else
		{
			overlap = Rect();
			return false;
		}
	}

	// LayoutImpl.h
	static constexpr Rect ExtendBy(const Rect& lhs, const Margin& rhs);
};