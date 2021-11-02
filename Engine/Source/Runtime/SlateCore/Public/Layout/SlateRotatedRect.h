// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

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

	constexpr SlateRotatedRect(const Rect& AlignedRect)
		: TopLeft(AlignedRect.GetLeftTop())
		, ExtentX(AlignedRect.Right - AlignedRect.Left, 0)
		, ExtentY(0, AlignedRect.Bottom - AlignedRect.Top)
	{
	}

	constexpr SlateRotatedRect(const Vector2& TopLeft, const Vector2& ExtentX, const Vector2& ExtentY)
		: TopLeft(TopLeft)
		, ExtentX(ExtentX)
		, ExtentY(ExtentY)
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

	constexpr bool IsUnderLocation(const Vector2& Location) const
	{
		const Vector2 Offset = Location - TopLeft;
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
	static constexpr SlateRotatedRect MakeRotatedRect(const Rect& ClipRectInLayoutWindowSpace, const SlateLayoutTransform& InverseLayoutTransform, const SlateRenderTransform& RenderTransform);
	static constexpr SlateRotatedRect MakeRotatedRect(const Rect& ClipRectInLayoutWindowSpace, const SlateRenderTransform& LayoutToRenderTransform);

	constexpr bool operator ==(const SlateRotatedRect& Rhs) const
	{
		return TopLeft == Rhs.TopLeft
			&& ExtentX == Rhs.ExtentX
			&& ExtentY == Rhs.ExtentY;
	}

	constexpr bool operator !=(const SlateRotatedRect& Rhs) const
	{
		return TopLeft != Rhs.TopLeft
			|| ExtentX != Rhs.ExtentX
			|| ExtentY != Rhs.ExtentY;
	}
};