// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateRenderTransform.h"
#include "SlateLayoutTransform.h"
#include "LayoutGeometry.h"
#include "SlateRotatedRect.h"
#include "Margin.h"
#include "PaintGeometry.h"
#include <optional>

class SWidget;
class ArrangedWidget;

struct APPENGINE_API Geometry
{
private:
	constexpr Geometry(
		const Vector2& InLocalSize,
		const SlateLayoutTransform& LocalLayoutTransform,
		const SlateRenderTransform& LocalRenderTransform,
		const Vector2& LocalRenderTransformPivot,
		const SlateLayoutTransform& ParentAccumulatedLayoutTransform,
		const SlateRenderTransform& ParentAccumulatedRenderTransform)
		: Size(InLocalSize)
		, Scale(1.0f)
		, AbsolutePosition(0.0f, 0.0f)
		, AccumulatedRenderTransform(
			TransformCalculus2D::Concatenate(
				// convert the pivot to local space and make it the origin
				TransformCalculus2D::Inverse(TransformCalculus2D::TransformPoint(Scale2D(InLocalSize), LocalRenderTransformPivot)),
				// apply the render transform in local space centered around the pivot
				LocalRenderTransform,
				// translate the pivot point back.
				TransformCalculus2D::TransformPoint(Scale2D(InLocalSize), LocalRenderTransformPivot),
				// apply the layout transform next.
				LocalLayoutTransform,
				// finally apply the parent accumulated transform, which takes us to the root.
				ParentAccumulatedRenderTransform
			)
		), bHasRenderTransform(true)
		, Position(LocalLayoutTransform.GetTranslation())
	{
		const SlateLayoutTransform AccumulatedLayoutTransform = TransformCalculus2D::Concatenate(LocalLayoutTransform, ParentAccumulatedLayoutTransform);
		const Vector2& Translation = AccumulatedLayoutTransform.GetTranslation();
		AbsolutePosition.Scalars[0] = Translation.Scalars[0];
		AbsolutePosition.Scalars[1] = Translation.Scalars[1];
		Scale = AccumulatedLayoutTransform.GetScale();
	}

	constexpr Geometry(
		const Vector2& InLocalSize,
		const SlateLayoutTransform& LocalLayoutTransform,
		const SlateLayoutTransform& ParentAccumulatedLayoutTransform,
		const SlateRenderTransform& ParentAccumulatedRenderTransform,
		bool bParentHasRenderTransform)
		: Size(InLocalSize)
		, Scale(1.0f)
		, AbsolutePosition(0.0f, 0.0f)
		, AccumulatedRenderTransform(TransformCalculus2D::Concatenate(LocalLayoutTransform, ParentAccumulatedRenderTransform))
		, bHasRenderTransform(bParentHasRenderTransform)
		, Position(LocalLayoutTransform.GetTranslation())
	{
		const SlateLayoutTransform AccumulatedLayoutTransform = TransformCalculus2D::Concatenate(LocalLayoutTransform, ParentAccumulatedLayoutTransform);
		const Vector2& Translation = AccumulatedLayoutTransform.GetTranslation();
		AbsolutePosition.Scalars[0] = Translation.Scalars[0];
		AbsolutePosition.Scalars[1] = Translation.Scalars[1];
		Scale = AccumulatedLayoutTransform.GetScale();
	}

public:
	constexpr Vector2 GetSize() const { return Size; }
	constexpr float GetScale() const { return Scale; }
	constexpr Vector2 GetAbsolutePosition() const { return AbsolutePosition; }
	constexpr Vector2 GetPosition() const { return Position; }

	constexpr bool operator ==(const Geometry& Rhs) const
	{
		return Size == Rhs.Size
			&& Scale == Rhs.Scale
			&& AbsolutePosition == Rhs.AbsolutePosition
			&& Position == Rhs.Position;
	}

	constexpr bool operator !=(const Geometry& Rhs) const
	{
		return !operator ==(Rhs);
	}

	constexpr Geometry MakeChild(const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform, const SlateRenderTransform& RenderTransform, const Vector2& RenderTransformPivot) const
	{
		return Geometry(LocalSize, LayoutTransform, RenderTransform, RenderTransformPivot, GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform());
	}

	constexpr Geometry MakeChild(const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform) const
	{
		return Geometry(LocalSize, LayoutTransform, GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform(), bHasRenderTransform);
	}

	constexpr Geometry MakeChild(const SlateRenderTransform& RenderTransform, const Vector2& RenderTransformPivot = Vector2(0.5f, 0.5f)) const
	{
		return Geometry(GetLocalSize(), SlateLayoutTransform(), RenderTransform, RenderTransformPivot, GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform());
	}

	constexpr Geometry MakeChild(const Vector2& ChildOffset, const Vector2& LocalSize, float ChildScale = 1.0f) const
	{
		return Geometry(LocalSize, SlateLayoutTransform(ChildScale, TransformCalculus2D::TransformPoint(ChildScale, ChildOffset)), GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform(), bHasRenderTransform);
	}

	ArrangedWidget MakeChild(SWidget* ChildWidget, const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform) const;
	ArrangedWidget MakeChild(SWidget* ChildWidget, const LayoutGeometry& LayoutGeometry) const;
	ArrangedWidget MakeChild(SWidget* ChildWidget, const Vector2& ChildOffset, const Vector2& LocalSize, float ChildScale = 1) const;

	constexpr PaintGeometry ToPaintGeometry() const
	{
		return PaintGeometry(GetAccumulatedLayoutTransform(), AccumulatedRenderTransform, Size, bHasRenderTransform);
	}

	constexpr PaintGeometry ToPaintGeometry(const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform) const
	{
		return PaintGeometry(
			TransformCalculus2D::Concatenate(LayoutTransform, GetAccumulatedLayoutTransform()),
			TransformCalculus2D::Concatenate(LayoutTransform, GetAccumulatedRenderTransform()),
			LocalSize,
			bHasRenderTransform
		);
	}

	constexpr PaintGeometry ToPaintGeometry(const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform, const SlateRenderTransform& RenderTransform, const Vector2& RenderTransformPivot) const
	{
		return MakeChild(LocalSize, LayoutTransform, RenderTransform, RenderTransformPivot).ToPaintGeometry();
	}

	constexpr PaintGeometry ToPaintGeometry(const SlateLayoutTransform& LayoutTransform) const
	{
		return ToPaintGeometry(Size, LayoutTransform);
	}

	constexpr PaintGeometry ToPaintGeometry(const Vector2& LocalOffset, const Vector2& LocalSize, float LocalScale)
	{
		return ToPaintGeometry(LocalSize, SlateLayoutTransform(LocalScale, Scale2D(LocalScale).TransformPoint(LocalOffset)));
	}

	// LayoutImpl.h
	constexpr bool IsUnderLocation(const Vector2& AbsoluteCoordinate) const;

	constexpr Vector2 AbsoluteToLocal(const Vector2& AbsoluteCoordinate) const
	{
		return TransformCalculus2D::TransformPoint(TransformCalculus2D::Inverse(AccumulatedRenderTransform), AbsoluteCoordinate);
	}

	constexpr Vector2 LocalToAbsolute(const Vector2& LocalCoordinate) const
	{
		return TransformCalculus2D::TransformPoint(AccumulatedRenderTransform, LocalCoordinate);
	}

	constexpr Vector2 LocalToRoundedLocal(const Vector2& LocalCoordinate) const
	{
		const Vector2 AbsoluteCoordinate = LocalToAbsolute(LocalCoordinate);
		const Vector2 AbsoluteCoordinateRounded = AbsoluteCoordinate.Round();
		return AbsoluteToLocal(AbsoluteCoordinateRounded);
	}

	constexpr Rect GetLayoutBoundingRect() const
	{
		return GetLayoutBoundingRect(Rect(Vector2::ZeroVector(), GetSize()));
	}

	constexpr Rect GetRenderBoundingRect() const
	{
		return GetRenderBoundingRect(Rect(Vector2::ZeroVector(), GetSize()));
	}

	// LayoutImpl.h
	constexpr Rect GetLayoutBoundingRect(const Rect& LocalSpaceRect) const;
	constexpr Rect GetRenderBoundingRect(const Rect& LocalSpaceRect) const;
	constexpr Rect GetLayoutBoundingRect(const Margin& LocalSpaceExtendBy) const;
	constexpr Rect GetRenderBoundingRect(const Margin& LocalSpaceExtendBy) const;

	constexpr SlateLayoutTransform GetAccumulatedLayoutTransform() const
	{
		return SlateLayoutTransform(Scale, AbsolutePosition);
	}

	constexpr const SlateRenderTransform& GetAccumulatedRenderTransform() const
	{
		return AccumulatedRenderTransform;
	}

	std::wstring ToString(std::wstring_view InFormatArgs = L"") const
	{
		std::wstring scaleText = std::format(StringUtils::GetPlaceholder(InFormatArgs), Scale);
		return std::format(L"[Abs={}, Scale={}, Size={}]", AbsolutePosition.ToString(InFormatArgs), scaleText, Size.ToString(InFormatArgs));
	}

	constexpr const Vector2& GetLocalSize() const
	{
		return Size;
	}

	static constexpr Geometry MakeRoot(const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform)
	{
		return Geometry(LocalSize, LayoutTransform, SlateLayoutTransform(), SlateRenderTransform(), false);
	}

	static constexpr Geometry MakeRoot(const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform, const SlateRenderTransform& RenderTransform)
	{
		return Geometry(LocalSize, LayoutTransform, SlateLayoutTransform(), RenderTransform, true);
	}

private:
	Vector2 Size;
	float Scale = 1.0f;
	Vector2 AbsolutePosition;
	Vector2 Position;
	SlateRenderTransform AccumulatedRenderTransform;
	bool bHasRenderTransform = false;
};