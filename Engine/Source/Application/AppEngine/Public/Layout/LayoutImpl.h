// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateRotatedRect.h"
#include "TransformCalculus2D.h"
#include "SlateRenderTransform.h"
#include "SlateLayoutTransform.h"
#include "Margin.h"
#include "Geometry.h"

template<class T>
static constexpr SlateRotatedRect TransformCalculus2D::TransformRect(const T& Transform, const SlateRotatedRect& Rect)
{
	return SlateRotatedRect
	(
		TransformPoint(Transform, Rect.TopLeft),
		TransformVector(Transform, Rect.ExtentX),
		TransformVector(Transform, Rect.ExtentY)
	);
}

constexpr SlateRotatedRect SlateRotatedRect::MakeRotatedRect(const Rect& ClipRectInLayoutWindowSpace, const SlateLayoutTransform& InverseLayoutTransform, const SlateRenderTransform& RenderTransform)
{
	return MakeRotatedRect(ClipRectInLayoutWindowSpace, TransformCalculus2D::Concatenate(InverseLayoutTransform, RenderTransform));
}

constexpr SlateRotatedRect SlateRotatedRect::MakeRotatedRect(const Rect& ClipRectInLayoutWindowSpace, const SlateRenderTransform& LayoutToRenderTransform)
{
	const SlateRotatedRect RotatedRect = TransformCalculus2D::TransformRect(LayoutToRenderTransform, ClipRectInLayoutWindowSpace);

	const Vector2 TopRight = RotatedRect.TopLeft + RotatedRect.ExtentX;
	const Vector2 BottomLeft = RotatedRect.TopLeft + RotatedRect.ExtentY;

	return SlateRotatedRect(
		RotatedRect.TopLeft,
		TopRight - RotatedRect.TopLeft,
		BottomLeft - RotatedRect.TopLeft);
}

constexpr SlateRenderTransform TransformCalculus2D::Concatenate(const SlateLayoutTransform& Layout, const SlateRenderTransform& Transform)
{
	return SlateRenderTransform(Layout.ToMatrix()).Concatenate(Transform);
}

constexpr SlateRenderTransform TransformCalculus2D::Concatenate(const Vector2& Translation, const SlateRenderTransform& Transform)
{
	return SlateRenderTransform(
		Transform.GetMatrix(),
		Concatenate(TransformPoint(Transform.GetMatrix(), Translation), Transform.GetTranslation())
	);
}

constexpr SlateRenderTransform TransformCalculus2D::Concatenate(const SlateRenderTransform& Transform, const Vector2& Translation)
{
	return SlateRenderTransform(
		Transform.GetMatrix(),
		Concatenate(Transform.GetTranslation(), Translation)
	);
}

constexpr Rect TransformCalculus2D::ExtendBy(const Rect& Lhs, const Margin& Rhs)
{
	return Rect(Lhs.Left - Rhs.Left, Lhs.Top - Rhs.Top, Lhs.Right + Rhs.Right, Lhs.Bottom + Rhs.Bottom);
}

constexpr bool Geometry::IsUnderLocation(const Vector2& AbsoluteCoordinate) const
{
	const SlateRotatedRect Rect = TransformCalculus2D::TransformRect(AccumulatedRenderTransform, SlateRotatedRect(::Rect(Vector2::ZeroVector(), GetSize())));
	return Rect.IsUnderLocation(AbsoluteCoordinate);
}

constexpr Rect Geometry::GetLayoutBoundingRect(const Rect& LocalSpaceRect) const
{
	return TransformCalculus2D::TransformRect(
		GetAccumulatedLayoutTransform(),
		SlateRotatedRect(LocalSpaceRect)).ToBoundingRect();
}

constexpr Rect Geometry::GetRenderBoundingRect(const Rect& LocalSpaceRect) const
{
	return TransformCalculus2D::TransformRect(
		GetAccumulatedRenderTransform(),
		SlateRotatedRect(LocalSpaceRect)).ToBoundingRect();
}

constexpr Rect Geometry::GetLayoutBoundingRect(const Margin& LocalSpaceExtendBy) const
{
	return TransformCalculus2D::ExtendBy(GetLayoutBoundingRect(Rect(Vector2::ZeroVector(), GetSize())), LocalSpaceExtendBy);
}

constexpr Rect Geometry::GetRenderBoundingRect(const Margin& LocalSpaceExtendBy) const
{
	return TransformCalculus2D::ExtendBy(GetRenderBoundingRect(Rect(Vector2::ZeroVector(), GetSize())), LocalSpaceExtendBy);
}