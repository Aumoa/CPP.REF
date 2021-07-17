// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "SlateRotatedRect.h"
#include "TransformCalculus2D.h"
#include "SlateRenderTransform.h"
#include "SlateLayoutTransform.h"
#include "Margin.h"
#include "Geometry.h"

template<LayoutTransform T>
static constexpr SlateRotatedRect TransformCalculus2D::TransformRect(const T& transform, const SlateRotatedRect& rect)
{
	return SlateRotatedRect
	(
		TransformPoint(transform, rect.TopLeft),
		TransformVector(transform, rect.ExtentX),
		TransformVector(transform, rect.ExtentY)
	);
}

constexpr SlateRotatedRect SlateRotatedRect::MakeRotatedRect(const Rect& clipRectInLayoutWindowSpace, const SlateLayoutTransform& inverseLayoutTransform, const SlateRenderTransform& renderTransform)
{
	return MakeRotatedRect(clipRectInLayoutWindowSpace, TransformCalculus2D::Concatenate(inverseLayoutTransform, renderTransform));
}

constexpr SlateRotatedRect SlateRotatedRect::MakeRotatedRect(const Rect& clipRectInLayoutWindowSpace, const SlateRenderTransform& layoutToRenderTransform)
{
	const SlateRotatedRect RotatedRect = TransformCalculus2D::TransformRect(layoutToRenderTransform, clipRectInLayoutWindowSpace);

	const Vector2 TopRight = RotatedRect.TopLeft + RotatedRect.ExtentX;
	const Vector2 BottomLeft = RotatedRect.TopLeft + RotatedRect.ExtentY;

	return SlateRotatedRect(
		RotatedRect.TopLeft,
		TopRight - RotatedRect.TopLeft,
		BottomLeft - RotatedRect.TopLeft);
}

constexpr SlateRenderTransform TransformCalculus2D::Concatenate(const SlateLayoutTransform& layout, const SlateRenderTransform& transform)
{
	return SlateRenderTransform(layout.ToMatrix()).Concatenate(transform);
}

constexpr SlateRenderTransform TransformCalculus2D::Concatenate(const Vector2& translation, const SlateRenderTransform& transform)
{
	return SlateRenderTransform(
		transform.GetMatrix(),
		Concatenate(TransformPoint(transform.GetMatrix(), translation), transform.GetTranslation())
	);
}

constexpr SlateRenderTransform TransformCalculus2D::Concatenate(const SlateRenderTransform& transform, const Vector2& translation)
{
	return SlateRenderTransform(
		transform.GetMatrix(),
		Concatenate(transform.GetTranslation(), translation)
	);
}

constexpr Rect TransformCalculus2D::ExtendBy(const Rect& lhs, const Margin& rhs)
{
	return Rect(lhs.Left - rhs.Left, lhs.Top - rhs.Top, lhs.Right + rhs.Right, lhs.Bottom + rhs.Bottom);
}

constexpr Rect Geometry::GetLayoutBoundingRect(const Margin& localSpaceExtendBy) const
{
	return TransformCalculus2D::ExtendBy(GetLayoutBoundingRect(Rect(Vector2::GetZero(), GetSize())), localSpaceExtendBy);
}

constexpr Rect Geometry::GetRenderBoundingRect(const Margin& localSpaceExtendBy) const
{
	return TransformCalculus2D::ExtendBy(GetRenderBoundingRect(Rect(Vector2::GetZero(), GetSize())), localSpaceExtendBy);
}