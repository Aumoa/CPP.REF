// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "SlateRenderTransform.h"
#include "TransformCalculus2D.h"

struct PaintGeometry
{
	constexpr PaintGeometry()
	{
	}

	constexpr PaintGeometry(const Vector2& localSize)
		: LocalSize(localSize)
	{
	}

	constexpr PaintGeometry(const Vector2& localSize, const SlateRenderTransform& accumulatedRenderTransform)
		: LocalSize(localSize)
		, AccumulatedRenderTransform(accumulatedRenderTransform)
		, bHasRenderTransform(true)
	{
	}

	constexpr PaintGeometry(const SlateLayoutTransform& accumulatedLayoutTransform, const SlateRenderTransform& accumulatedRenderTransform, const Vector2& localSize, bool bHasRenderTransform)
		: LocalSize(localSize)
		, AccumulatedRenderTransform(accumulatedRenderTransform)
		, bHasRenderTransform(bHasRenderTransform)
	{
	}

	constexpr PaintGeometry AppendTransform(const SlateLayoutTransform& layoutTransform) const
	{
		return PaintGeometry(LocalSize, TransformCalculus2D::Concatenate(AccumulatedRenderTransform, layoutTransform));
	}

	PaintGeometry& AppendTransform(const SlateLayoutTransform& layoutTransform)
	{
		check(bHasRenderTransform);
		AccumulatedRenderTransform = TransformCalculus2D::Concatenate(AccumulatedRenderTransform, layoutTransform);
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

	PaintGeometry& SetRenderTransform(const SlateRenderTransform& renderTransform)
	{
		AccumulatedRenderTransform = renderTransform;
		bHasRenderTransform = true;
		return *this;
	}

	constexpr bool operator ==(const PaintGeometry& rhs) const
	{
		return LocalSize == rhs.LocalSize
			&& (bHasRenderTransform ? (rhs.bHasRenderTransform && AccumulatedRenderTransform == rhs.AccumulatedRenderTransform) : !rhs.bHasRenderTransform);
	}

	constexpr bool operator !=(const PaintGeometry& rhs) const
	{
		return !operator ==(rhs);
	}

	static constexpr PaintGeometry Identity()
	{
		return PaintGeometry(Vector2::GetZero());
	}

private:
	Vector2 LocalSize;
	SlateRenderTransform AccumulatedRenderTransform;
	bool bHasRenderTransform = false;
};