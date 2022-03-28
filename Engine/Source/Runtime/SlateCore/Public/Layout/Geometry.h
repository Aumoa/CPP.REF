// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreNumerics.h"
#include "SlateRenderTransform.h"
#include "SlateLayoutTransform.h"
#include "LayoutGeometry.h"
#include "Margin.h"
#include "PaintGeometry.h"
#include <optional>

class SWidget;
class ArrangedWidget;

struct SLATECORE_API Geometry
{
private:
	constexpr Geometry(
		const Scale2D& LocalSize,
		const SlateLayoutTransform& LocalLayoutTransform,
		const SlateRenderTransform& LocalRenderTransform,
		const Translate2D& LocalRenderTransformPivot,
		const SlateLayoutTransform& ParentAccumulatedLayoutTransform,
		const SlateRenderTransform& ParentAccumulatedRenderTransform)
		: Size(LocalSize)
		, Scale(1.0f)
		, AbsolutePosition(0.0f, 0.0f)
		, AccumulatedRenderTransform(
			Transform::Concatenate(
				// convert the pivot to local space and make it the origin
				LocalSize.TransformPoint(LocalRenderTransformPivot).Inverse(),
				// apply the render transform in local space centered around the pivot
				LocalRenderTransform,
				// translate the pivot point back.
				LocalSize.TransformPoint(LocalRenderTransformPivot),
				// apply the layout transform next.
				LocalLayoutTransform,
				// finally apply the parent accumulated transform, which takes us to the root.
				ParentAccumulatedRenderTransform
			)
		), bHasRenderTransform(true)
		, Position(LocalLayoutTransform.Translation)
	{
		const SlateLayoutTransform AccumulatedLayoutTransform = Transform::Concatenate(LocalLayoutTransform, ParentAccumulatedLayoutTransform);
		const Translate2D& Translation = AccumulatedLayoutTransform.Translation;
		AbsolutePosition.X = Translation.X;
		AbsolutePosition.Y = Translation.Y;
		Scale = AccumulatedLayoutTransform.Scale;
	}

	constexpr Geometry(
		const Vector2& LocalSize,
		const SlateLayoutTransform& LocalLayoutTransform,
		const SlateLayoutTransform& ParentAccumulatedLayoutTransform,
		const SlateRenderTransform& ParentAccumulatedRenderTransform,
		bool bParentHasRenderTransform)
		: Size(LocalSize)
		, Scale(1.0f)
		, AbsolutePosition(0.0f, 0.0f)
		, AccumulatedRenderTransform(Transform::Concatenate(LocalLayoutTransform, ParentAccumulatedRenderTransform))
		, bHasRenderTransform(bParentHasRenderTransform)
		, Position(LocalLayoutTransform.Translation)
	{
		const SlateLayoutTransform AccumulatedLayoutTransform = Transform::Concatenate(LocalLayoutTransform, ParentAccumulatedLayoutTransform);
		const Translate2D& Translation = AccumulatedLayoutTransform.Translation;
		AbsolutePosition.X = Translation.X;
		AbsolutePosition.Y = Translation.Y;
		Scale = AccumulatedLayoutTransform.Scale;
	}

public:
	constexpr Vector2 GetSize() const { return Size; }
	constexpr Scale2D GetScale() const { return Scale; }
	constexpr Vector2 GetAbsolutePosition() const { return AbsolutePosition; }
	constexpr Vector2 GetPosition() const { return Position; }

	constexpr auto operator <=>(const Geometry& Rhs) const = default;

	constexpr Geometry MakeChild(const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform, const SlateRenderTransform& RenderTransform, const Translate2D& RenderTransformPivot) const
	{
		return Geometry(
			LocalSize,
			LayoutTransform,
			RenderTransform,
			RenderTransformPivot,
			GetAccumulatedLayoutTransform(),
			GetAccumulatedRenderTransform()
		);
	}

	constexpr Geometry MakeChild(const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform) const
	{
		return Geometry(
			LocalSize,
			LayoutTransform,
			GetAccumulatedLayoutTransform(),
			GetAccumulatedRenderTransform(),
			bHasRenderTransform
		);
	}

	constexpr Geometry MakeChild(const SlateRenderTransform& RenderTransform, const Translate2D& RenderTransformPivot = Translate2D(0.5f, 0.5f)) const
	{
		return Geometry(
			GetLocalSize(),
			SlateLayoutTransform(),
			RenderTransform,
			RenderTransformPivot,
			GetAccumulatedLayoutTransform(),
			GetAccumulatedRenderTransform()
		);
	}

	constexpr Geometry MakeChild(const Vector2& ChildOffset, const Vector2& LocalSize, float ChildScale = 1.0f) const
	{
		return Geometry(LocalSize, SlateLayoutTransform(ChildScale, Scale2D(ChildScale).TransformPoint(ChildOffset)), GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform(), bHasRenderTransform);
	}

	ArrangedWidget MakeChild(SWidget* ChildWidget, const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform) const;
	ArrangedWidget MakeChild(SWidget* ChildWidget, const LayoutGeometry& LayoutGeometry) const;
	ArrangedWidget MakeChild(SWidget* ChildWidget, const Translate2D& ChildOffset, const Vector2& LocalSize, const Scale2D& ChildScale = Scale2D::Identity()) const;

	constexpr PaintGeometry ToPaintGeometry() const
	{
		return PaintGeometry(AccumulatedRenderTransform, Size, bHasRenderTransform);
	}

	constexpr PaintGeometry ToPaintGeometry(const Vector2& LocalSize, const SlateLayoutTransform& LayoutTransform) const
	{
		return PaintGeometry(
			Transform::Concatenate(LayoutTransform, GetAccumulatedRenderTransform()),
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

	constexpr PaintGeometry ToPaintGeometry(const Translate2D& LocalOffset, const Vector2& LocalSize, const Scale2D& LocalScale)
	{
		return ToPaintGeometry(LocalSize, SlateLayoutTransform(LocalScale, LocalScale.TransformPoint(LocalOffset)));
	}

	constexpr bool IsUnderLocation(const Translate2D& AbsoluteCoordinate) const
	{
		const RotatedRect RRect = RotatedRect::TransformRect(AccumulatedRenderTransform, RotatedRect(Rect(Vector2::Zero(), GetSize())));
		return RRect.IsUnderLocation(AbsoluteCoordinate);
	}

	constexpr Translate2D AbsoluteToLocal(const Translate2D& AbsoluteCoordinate) const
	{
		return AccumulatedRenderTransform.Inverse().TransformPoint(AbsoluteCoordinate);
	}

	constexpr Translate2D LocalToAbsolute(const Translate2D& LocalCoordinate) const
	{
		return AccumulatedRenderTransform.TransformPoint(LocalCoordinate);
	}

	constexpr Translate2D LocalToRoundedLocal(const Translate2D& LocalCoordinate) const
	{
		const Translate2D AbsoluteCoordinate = LocalToAbsolute(LocalCoordinate);
		const Translate2D AbsoluteCoordinateRounded = AbsoluteCoordinate.Round();
		return AbsoluteToLocal(AbsoluteCoordinateRounded);
	}

	constexpr Rect GetLayoutBoundingRect() const
	{
		return GetLayoutBoundingRect(Rect(Vector2::Zero(), GetSize()));
	}

	constexpr Rect GetRenderBoundingRect() const
	{
		return GetRenderBoundingRect(Rect(Vector2::Zero(), GetSize()));
	}

	constexpr Rect GetLayoutBoundingRect(const Rect& LocalSpaceRect) const
	{
		return RotatedRect::TransformRect(
			GetAccumulatedLayoutTransform(),
			RotatedRect(LocalSpaceRect)).ToBoundingRect();
	}

	constexpr Rect GetRenderBoundingRect(const Rect& LocalSpaceRect) const
	{
		return RotatedRect::TransformRect(
			GetAccumulatedRenderTransform(),
			RotatedRect(LocalSpaceRect)).ToBoundingRect();
	}

	constexpr Rect GetLayoutBoundingRect(const Margin& LocalSpaceExtendBy) const
	{
		return GetLayoutBoundingRect(Rect(Vector2::Zero(), GetSize())).Extend(LocalSpaceExtendBy);
	}

	constexpr Rect GetRenderBoundingRect(const Margin& LocalSpaceExtendBy) const
	{
		return GetRenderBoundingRect(Rect(Vector2::Zero(), GetSize())).Extend(LocalSpaceExtendBy);
	}

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
		std::wstring scaleText = String::Format(String::GetPlaceholder(InFormatArgs), Scale.ToString(InFormatArgs));
		return String::Format(L"[Abs={}, Scale={}, Size={}]", AbsolutePosition.ToString(InFormatArgs), scaleText, Size.ToString(InFormatArgs));
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
	Scale2D Scale = Scale2D::Identity();
	Translate2D AbsolutePosition;
	Translate2D Position;
	SlateRenderTransform AccumulatedRenderTransform;
	bool bHasRenderTransform = false;
};