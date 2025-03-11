// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/SlateRenderTransform.h"
#include "Layout/SlateLayoutTransform.h"
#include "Layout/LayoutGeometry.h"
#include "Layout/Margin.h"
#include "Layout/PaintGeometry.h"
#include "Numerics/VectorInterface/Rect.h"
#include "Numerics/MatrixInterface/RotatedRect.h"
#include <optional>

class SWidget;
class NArrangedWidget;

struct SLATECORE_API NGeometry
{
private:
	constexpr NGeometry(
		const Scale2D& LocalSize,
		const NSlateLayoutTransform& LocalLayoutTransform,
		const NSlateRenderTransform& LocalRenderTransform,
		const Translate2D& LocalRenderTransformPivot,
		const NSlateLayoutTransform& ParentAccumulatedLayoutTransform,
		const NSlateRenderTransform& ParentAccumulatedRenderTransform)
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
		const NSlateLayoutTransform AccumulatedLayoutTransform = Transform::Concatenate(LocalLayoutTransform, ParentAccumulatedLayoutTransform);
		const Translate2D& Translation = AccumulatedLayoutTransform.Translation;
		AbsolutePosition.X = Translation.X;
		AbsolutePosition.Y = Translation.Y;
		Scale = AccumulatedLayoutTransform.Scale;
	}

	constexpr NGeometry(
		const Vector2& LocalSize,
		const NSlateLayoutTransform& LocalLayoutTransform,
		const NSlateLayoutTransform& ParentAccumulatedLayoutTransform,
		const NSlateRenderTransform& ParentAccumulatedRenderTransform,
		bool bParentHasRenderTransform)
		: Size(LocalSize)
		, Scale(1.0f)
		, AbsolutePosition(0.0f, 0.0f)
		, AccumulatedRenderTransform(Transform::Concatenate(LocalLayoutTransform, ParentAccumulatedRenderTransform))
		, bHasRenderTransform(bParentHasRenderTransform)
		, Position(LocalLayoutTransform.Translation)
	{
		const NSlateLayoutTransform AccumulatedLayoutTransform = Transform::Concatenate(LocalLayoutTransform, ParentAccumulatedLayoutTransform);
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

	constexpr auto operator <=>(const NGeometry& Rhs) const = default;

	constexpr NGeometry MakeChild(const Vector2& LocalSize, const NSlateLayoutTransform& LayoutTransform, const NSlateRenderTransform& RenderTransform, const Translate2D& RenderTransformPivot) const
	{
		return NGeometry(
			LocalSize,
			LayoutTransform,
			RenderTransform,
			RenderTransformPivot,
			GetAccumulatedLayoutTransform(),
			GetAccumulatedRenderTransform()
		);
	}

	constexpr NGeometry MakeChild(const Vector2& LocalSize, const NSlateLayoutTransform& LayoutTransform) const
	{
		return NGeometry(
			LocalSize,
			LayoutTransform,
			GetAccumulatedLayoutTransform(),
			GetAccumulatedRenderTransform(),
			bHasRenderTransform
		);
	}

	constexpr NGeometry MakeChild(const NSlateRenderTransform& RenderTransform, const Translate2D& RenderTransformPivot = Translate2D(0.5f, 0.5f)) const
	{
		return NGeometry(
			GetLocalSize(),
			NSlateLayoutTransform(),
			RenderTransform,
			RenderTransformPivot,
			GetAccumulatedLayoutTransform(),
			GetAccumulatedRenderTransform()
		);
	}

	constexpr NGeometry MakeChild(const Vector2& ChildOffset, const Vector2& LocalSize, float ChildScale = 1.0f) const
	{
		return NGeometry(LocalSize, NSlateLayoutTransform(ChildScale, Scale2D(ChildScale).TransformPoint(ChildOffset)), GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform(), bHasRenderTransform);
	}

	NArrangedWidget MakeChild(std::shared_ptr<SWidget> ChildWidget, const Vector2& LocalSize, const NSlateLayoutTransform& LayoutTransform) const;
	NArrangedWidget MakeChild(std::shared_ptr<SWidget> ChildWidget, const NLayoutGeometry& LayoutGeometry) const;
	NArrangedWidget MakeChild(std::shared_ptr<SWidget> ChildWidget, const Translate2D& ChildOffset, const Vector2& LocalSize, const Scale2D& ChildScale = Scale2D::Identity()) const;

	constexpr NPaintGeometry ToPaintGeometry() const
	{
		return NPaintGeometry(AccumulatedRenderTransform, Size, bHasRenderTransform);
	}

	constexpr NPaintGeometry ToPaintGeometry(const Vector2& LocalSize, const NSlateLayoutTransform& LayoutTransform) const
	{
		return NPaintGeometry(
			Transform::Concatenate(LayoutTransform, GetAccumulatedRenderTransform()),
			LocalSize,
			bHasRenderTransform
		);
	}

	constexpr NPaintGeometry ToPaintGeometry(const Vector2& LocalSize, const NSlateLayoutTransform& LayoutTransform, const NSlateRenderTransform& RenderTransform, const Vector2& RenderTransformPivot) const
	{
		return MakeChild(LocalSize, LayoutTransform, RenderTransform, RenderTransformPivot).ToPaintGeometry();
	}

	constexpr NPaintGeometry ToPaintGeometry(const NSlateLayoutTransform& LayoutTransform) const
	{
		return ToPaintGeometry(Size, LayoutTransform);
	}

	constexpr NPaintGeometry ToPaintGeometry(const Translate2D& LocalOffset, const Vector2& LocalSize, const Scale2D& LocalScale)
	{
		return ToPaintGeometry(LocalSize, NSlateLayoutTransform(LocalScale, LocalScale.TransformPoint(LocalOffset)));
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

	constexpr Rect GetLayoutBoundingRect(const NMargin& LocalSpaceExtendBy) const
	{
		return GetLayoutBoundingRect(Rect(Vector2::Zero(), GetSize())).Extend(LocalSpaceExtendBy);
	}

	constexpr Rect GetRenderBoundingRect(const NMargin& LocalSpaceExtendBy) const
	{
		return GetRenderBoundingRect(Rect(Vector2::Zero(), GetSize())).Extend(LocalSpaceExtendBy);
	}

	constexpr NSlateLayoutTransform GetAccumulatedLayoutTransform() const
	{
		return NSlateLayoutTransform(Scale, AbsolutePosition);
	}

	constexpr const NSlateRenderTransform& GetAccumulatedRenderTransform() const
	{
		return AccumulatedRenderTransform;
	}

	String ToString(String InFormatArgs = TEXT("")) const
	{
		String ScaleText = Scale.ToString();
		return String::Format(TEXT("[Abs={}, Scale={}, Size={}]"), AbsolutePosition.ToString(), ScaleText, Size.ToString());
	}

	constexpr const Vector2& GetLocalSize() const
	{
		return Size;
	}

	static constexpr NGeometry MakeRoot(const Vector2& LocalSize, const NSlateLayoutTransform& LayoutTransform)
	{
		return NGeometry(LocalSize, LayoutTransform, NSlateLayoutTransform(), NSlateRenderTransform(), false);
	}

	static constexpr NGeometry MakeRoot(const Vector2& LocalSize, const NSlateLayoutTransform& LayoutTransform, const NSlateRenderTransform& RenderTransform)
	{
		return NGeometry(LocalSize, LayoutTransform, NSlateLayoutTransform(), RenderTransform, true);
	}

private:
	Vector2 Size;
	Scale2D Scale = Scale2D::Identity();
	Translate2D AbsolutePosition;
	Translate2D Position;
	NSlateRenderTransform AccumulatedRenderTransform;
	bool bHasRenderTransform = false;
};