// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/SlateRenderTransform.h"
#include "Layout/SlateLayoutTransform.h"

struct NPaintGeometry
{
	constexpr NPaintGeometry()
	{
	}

	constexpr NPaintGeometry(const Vector2& LocalSize)
		: LocalSize(LocalSize)
	{
	}

	constexpr NPaintGeometry(const Vector2& LocalSize, const NSlateRenderTransform& AccumulatedRenderTransform)
		: LocalSize(LocalSize)
		, AccumulatedRenderTransform(AccumulatedRenderTransform)
		, bHasRenderTransform(true)
	{
	}

	constexpr NPaintGeometry(const NSlateRenderTransform& AccumulatedRenderTransform, const Vector2& LocalSize, bool bHasRenderTransform)
		: LocalSize(LocalSize)
		, AccumulatedRenderTransform(AccumulatedRenderTransform)
		, bHasRenderTransform(bHasRenderTransform)
	{
	}

	constexpr NPaintGeometry AppendTransform(const NSlateLayoutTransform& LayoutTransform) const
	{
		return NPaintGeometry(LocalSize, AccumulatedRenderTransform.Concatenate(LayoutTransform));
	}

	NPaintGeometry& AppendTransform(const NSlateLayoutTransform& LayoutTransform)
	{
		checkf(bHasRenderTransform, TEXT("Geometry haven't render transform."));
		AccumulatedRenderTransform = Transform::Concatenate(AccumulatedRenderTransform, LayoutTransform);
		return *this;
	}

	constexpr const Vector2& GetLocalSize() const
	{
		return LocalSize;
	}

	constexpr const NSlateRenderTransform& GetAccumulatedRenderTransform() const
	{
		return AccumulatedRenderTransform;
	}

	constexpr bool HasRenderTransform() const
	{
		return bHasRenderTransform;
	}

	NPaintGeometry& SetRenderTransform(const NSlateRenderTransform& RenderTransform)
	{
		AccumulatedRenderTransform = RenderTransform;
		bHasRenderTransform = true;
		return *this;
	}

	constexpr bool operator ==(const NPaintGeometry& Rhs) const
	{
		return LocalSize == Rhs.LocalSize
			&& (bHasRenderTransform ? (Rhs.bHasRenderTransform && AccumulatedRenderTransform == Rhs.AccumulatedRenderTransform) : !Rhs.bHasRenderTransform);
	}

	constexpr bool operator !=(const NPaintGeometry& Rhs) const
	{
		return !operator ==(Rhs);
	}

	static constexpr NPaintGeometry Identity()
	{
		return NPaintGeometry(Vector2::Zero());
	}

private:
	Vector2 LocalSize;
	NSlateRenderTransform AccumulatedRenderTransform;
	bool bHasRenderTransform = false;
};