// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateRenderTransform.h"
#include "SlateLayoutTransform.h"

struct PaintGeometry
{
	constexpr PaintGeometry()
	{
	}

	constexpr PaintGeometry(const Vector2& LocalSize)
		: LocalSize(LocalSize)
	{
	}

	constexpr PaintGeometry(const Vector2& LocalSize, const SlateRenderTransform& AccumulatedRenderTransform)
		: LocalSize(LocalSize)
		, AccumulatedRenderTransform(AccumulatedRenderTransform)
		, bHasRenderTransform(true)
	{
	}

	constexpr PaintGeometry(const SlateRenderTransform& AccumulatedRenderTransform, const Vector2& LocalSize, bool bHasRenderTransform)
		: LocalSize(LocalSize)
		, AccumulatedRenderTransform(AccumulatedRenderTransform)
		, bHasRenderTransform(bHasRenderTransform)
	{
	}

	constexpr PaintGeometry AppendTransform(const SlateLayoutTransform& LayoutTransform) const
	{
		return PaintGeometry(LocalSize, AccumulatedRenderTransform.Concatenate(LayoutTransform));
	}

	PaintGeometry& AppendTransform(const SlateLayoutTransform& LayoutTransform)
	{
		checkf(bHasRenderTransform, TEXT("Geometry haven't render transform."));
		AccumulatedRenderTransform = Transform::Concatenate(AccumulatedRenderTransform, LayoutTransform);
		return *this;
	}

	constexpr const Vector2& GetLocalSize() const
	{
		return LocalSize;
	}

	constexpr const SlateRenderTransform& GetAccumulatedRenderTransform() const
	{
		return AccumulatedRenderTransform;
	}

	constexpr bool HasRenderTransform() const
	{
		return bHasRenderTransform;
	}

	PaintGeometry& SetRenderTransform(const SlateRenderTransform& RenderTransform)
	{
		AccumulatedRenderTransform = RenderTransform;
		bHasRenderTransform = true;
		return *this;
	}

	constexpr bool operator ==(const PaintGeometry& Rhs) const
	{
		return LocalSize == Rhs.LocalSize
			&& (bHasRenderTransform ? (Rhs.bHasRenderTransform && AccumulatedRenderTransform == Rhs.AccumulatedRenderTransform) : !Rhs.bHasRenderTransform);
	}

	constexpr bool operator !=(const PaintGeometry& Rhs) const
	{
		return !operator ==(Rhs);
	}

	static constexpr PaintGeometry Identity()
	{
		return PaintGeometry(Vector2::Zero());
	}

private:
	Vector2 LocalSize;
	SlateRenderTransform AccumulatedRenderTransform;
	bool bHasRenderTransform = false;
};