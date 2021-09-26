// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

struct SlateLayoutTransform;
struct SlateRenderTransform;

struct SlateRotatedRect
{
	Vector2 TopLeft;
	Vector2 ExtentX;
	Vector2 ExtentY;

	constexpr SlateRotatedRect()
	{
	}

	constexpr SlateRotatedRect(const Rect& alignedRect)
		: TopLeft(alignedRect.GetLeftTop())
		, ExtentX(alignedRect.Right - alignedRect.Left, 0)
		, ExtentY(0, alignedRect.Bottom - alignedRect.Top)
	{
	}

	constexpr SlateRotatedRect(const Vector2& topLeft, const Vector2& extentX, const Vector2& extentY)
		: TopLeft(topLeft)
		, ExtentX(extentX)
		, ExtentY(extentY)
	{
	}

	constexpr Rect ToBoundingRect() const
	{
		const Vector2 Points[] =
		{
			TopLeft,
			TopLeft + ExtentX,
			TopLeft + ExtentY,
			TopLeft + ExtentX + ExtentY
		};

		return Rect(
			MathEx::Min(MathEx::Min(MathEx::Min(Points[0].X, Points[1].X), Points[2].X), Points[3].X),
			MathEx::Min(MathEx::Min(MathEx::Min(Points[0].Y, Points[1].Y), Points[2].Y), Points[3].Y),
			MathEx::Max(MathEx::Max(MathEx::Max(Points[0].X, Points[1].X), Points[2].X), Points[3].X),
			MathEx::Max(MathEx::Max(MathEx::Max(Points[0].Y, Points[1].Y), Points[2].Y), Points[3].Y)
		);
	}

	constexpr bool IsUnderLocation(const Vector2& location) const
	{
		const Vector2 Offset = location - TopLeft;
		const float Det = Vector2::CrossProduct(ExtentX, ExtentY);

		const float S = -Vector2::CrossProduct(Offset, ExtentX) / Det;
		if (MathEx::IsWithinInclusive(S, 0.0f, 1.0f))
		{
			const float T = Vector2::CrossProduct(Offset, ExtentY) / Det;
			return MathEx::IsWithinInclusive(T, 0.0f, 1.0f);
		}
		return false;
	}

	// LayoutImpl.h
	static constexpr SlateRotatedRect MakeRotatedRect(const Rect& clipRectInLayoutWindowSpace, const SlateLayoutTransform& inverseLayoutTransform, const SlateRenderTransform& renderTransform);
	static constexpr SlateRotatedRect MakeRotatedRect(const Rect& clipRectInLayoutWindowSpace, const SlateRenderTransform& layoutToRenderTransform);

	constexpr bool operator ==(const SlateRotatedRect& rhs) const
	{
		return TopLeft == rhs.TopLeft
			&& ExtentX == rhs.ExtentX
			&& ExtentY == rhs.ExtentY;
	}

	constexpr bool operator !=(const SlateRotatedRect& rhs) const
	{
		return TopLeft != rhs.TopLeft
			|| ExtentX != rhs.ExtentX
			|| ExtentY != rhs.ExtentY;
	}
};