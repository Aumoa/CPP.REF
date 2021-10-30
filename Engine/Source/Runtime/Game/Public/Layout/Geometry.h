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

struct GAME_API Geometry
{
private:
	constexpr Geometry(
		const Vector2& localSize,
		const SlateLayoutTransform& localLayoutTransform,
		const SlateRenderTransform& localRenderTransform,
		const Vector2& localRenderTransformPivot,
		const SlateLayoutTransform& parentAccumulatedLayoutTransform,
		const SlateRenderTransform& parentAccumulatedRenderTransform)
		: Size(localSize)
		, Scale(1.0f)
		, AbsolutePosition(0.0f, 0.0f)
		, AccumulatedRenderTransform(
			TransformCalculus2D::Concatenate(
				// convert the pivot to local space and make it the origin
				TransformCalculus2D::Inverse(TransformCalculus2D::TransformPoint(Scale2D(localSize), localRenderTransformPivot)),
				// apply the render transform in local space centered around the pivot
				localRenderTransform,
				// translate the pivot point back.
				TransformCalculus2D::TransformPoint(Scale2D(localSize), localRenderTransformPivot),
				// apply the layout transform next.
				localLayoutTransform,
				// finally apply the parent accumulated transform, which takes us to the root.
				parentAccumulatedRenderTransform
			)
		), bHasRenderTransform(true)
		, Position(localLayoutTransform.GetTranslation())
	{
		const SlateLayoutTransform AccumulatedLayoutTransform = TransformCalculus2D::Concatenate(localLayoutTransform, parentAccumulatedLayoutTransform);
		const Vector2& Translation = AccumulatedLayoutTransform.GetTranslation();
		AbsolutePosition.Scalars[0] = Translation.Scalars[0];
		AbsolutePosition.Scalars[1] = Translation.Scalars[1];
		Scale = AccumulatedLayoutTransform.GetScale();
	}

	constexpr Geometry(
		const Vector2& localSize,
		const SlateLayoutTransform& localLayoutTransform,
		const SlateLayoutTransform& parentAccumulatedLayoutTransform,
		const SlateRenderTransform& parentAccumulatedRenderTransform,
		bool bParentHasRenderTransform)
		: Size(localSize)
		, Scale(1.0f)
		, AbsolutePosition(0.0f, 0.0f)
		, AccumulatedRenderTransform(TransformCalculus2D::Concatenate(localLayoutTransform, parentAccumulatedRenderTransform))
		, bHasRenderTransform(bParentHasRenderTransform)
		, Position(localLayoutTransform.GetTranslation())
	{
		const SlateLayoutTransform AccumulatedLayoutTransform = TransformCalculus2D::Concatenate(localLayoutTransform, parentAccumulatedLayoutTransform);
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

	constexpr bool operator ==(const Geometry& rhs) const
	{
		return Size == rhs.Size
			&& Scale == rhs.Scale
			&& AbsolutePosition == rhs.AbsolutePosition
			&& Position == rhs.Position;
	}

	constexpr bool operator !=(const Geometry& rhs) const
	{
		return !operator ==(rhs);
	}

	constexpr Geometry MakeChild(const Vector2& localSize, const SlateLayoutTransform& layoutTransform, const SlateRenderTransform& renderTransform, const Vector2& renderTransformPivot) const
	{
		return Geometry(localSize, layoutTransform, renderTransform, renderTransformPivot, GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform());
	}

	constexpr Geometry MakeChild(const Vector2& localSize, const SlateLayoutTransform& layoutTransform) const
	{
		return Geometry(localSize, layoutTransform, GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform(), bHasRenderTransform);
	}

	constexpr Geometry MakeChild(const SlateRenderTransform& renderTransform, const Vector2& renderTransformPivot = Vector2(0.5f, 0.5f)) const
	{
		return Geometry(GetLocalSize(), SlateLayoutTransform(), renderTransform, renderTransformPivot, GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform());
	}

	constexpr Geometry MakeChild(const Vector2& childOffset, const Vector2& localSize, float childScale = 1.0f) const
	{
		return Geometry(localSize, SlateLayoutTransform(childScale, TransformCalculus2D::TransformPoint(childScale, childOffset)), GetAccumulatedLayoutTransform(), GetAccumulatedRenderTransform(), bHasRenderTransform);
	}

	ArrangedWidget MakeChild(SWidget* childWidget, const Vector2& localSize, const SlateLayoutTransform& layoutTransform) const;
	ArrangedWidget MakeChild(SWidget* childWidget, const LayoutGeometry& layoutGeometry) const;
	ArrangedWidget MakeChild(SWidget* childWidget, const Vector2& childOffset, const Vector2& localSize, float childScale = 1) const;

	constexpr PaintGeometry ToPaintGeometry() const
	{
		return PaintGeometry(GetAccumulatedLayoutTransform(), AccumulatedRenderTransform, Size, bHasRenderTransform);
	}

	constexpr PaintGeometry ToPaintGeometry(const Vector2& localSize, const SlateLayoutTransform& layoutTransform) const
	{
		return PaintGeometry(
			TransformCalculus2D::Concatenate(layoutTransform, GetAccumulatedLayoutTransform()),
			TransformCalculus2D::Concatenate(layoutTransform, GetAccumulatedRenderTransform()),
			localSize,
			bHasRenderTransform
		);
	}

	constexpr PaintGeometry ToPaintGeometry(const Vector2& localSize, const SlateLayoutTransform& layoutTransform, const SlateRenderTransform& renderTransform, const Vector2& renderTransformPivot) const
	{
		return MakeChild(localSize, layoutTransform, renderTransform, renderTransformPivot).ToPaintGeometry();
	}

	constexpr PaintGeometry ToPaintGeometry(const SlateLayoutTransform& layoutTransform) const
	{
		return ToPaintGeometry(Size, layoutTransform);
	}

	constexpr PaintGeometry ToPaintGeometry(const Vector2& localOffset, const Vector2& localSize, float localScale)
	{
		return ToPaintGeometry(localSize, SlateLayoutTransform(localScale, Scale2D(localScale).TransformPoint(localOffset)));
	}

	// LayoutImpl.h
	constexpr bool IsUnderLocation(const Vector2& absoluteCoordinate) const;

	constexpr Vector2 AbsoluteToLocal(const Vector2& absoluteCoordinate) const
	{
		return TransformCalculus2D::TransformPoint(TransformCalculus2D::Inverse(AccumulatedRenderTransform), absoluteCoordinate);
	}

	constexpr Vector2 LocalToAbsolute(const Vector2& localCoordinate) const
	{
		return TransformCalculus2D::TransformPoint(AccumulatedRenderTransform, localCoordinate);
	}

	constexpr Vector2 LocalToRoundedLocal(const Vector2& localCoordinate) const
	{
		const Vector2 AbsoluteCoordinate = LocalToAbsolute(localCoordinate);
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
	constexpr Rect GetLayoutBoundingRect(const Rect& localSpaceRect) const;
	constexpr Rect GetRenderBoundingRect(const Rect& localSpaceRect) const;
	constexpr Rect GetLayoutBoundingRect(const Margin& localSpaceExtendBy) const;
	constexpr Rect GetRenderBoundingRect(const Margin& localSpaceExtendBy) const;

	constexpr SlateLayoutTransform GetAccumulatedLayoutTransform() const
	{
		return SlateLayoutTransform(Scale, AbsolutePosition);
	}

	constexpr const SlateRenderTransform& GetAccumulatedRenderTransform() const
	{
		return AccumulatedRenderTransform;
	}

	std::wstring ToString(std::wstring_view formatArgs = L"") const
	{
		std::wstring scaleText = std::format(StringUtils::GetPlaceholder(formatArgs), Scale);
		return std::format(L"[Abs={}, Scale={}, Size={}]", AbsolutePosition.ToString(formatArgs), scaleText, Size.ToString(formatArgs));
	}

	constexpr const Vector2& GetLocalSize() const
	{
		return Size;
	}

	static constexpr Geometry MakeRoot(const Vector2& localSize, const SlateLayoutTransform& layoutTransform)
	{
		return Geometry(localSize, layoutTransform, SlateLayoutTransform(), SlateRenderTransform(), false);
	}

	static constexpr Geometry MakeRoot(const Vector2& localSize, const SlateLayoutTransform& layoutTransform, const SlateRenderTransform& renderTransform)
	{
		return Geometry(localSize, layoutTransform, SlateLayoutTransform(), renderTransform, true);
	}

private:
	Vector2 Size;
	float Scale = 1.0f;
	Vector2 AbsolutePosition;
	Vector2 Position;
	SlateRenderTransform AccumulatedRenderTransform;
	bool bHasRenderTransform = false;
};