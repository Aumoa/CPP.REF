// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct Anchors
{
	Vector2 Minimum;
	Vector2 Maximum;

	constexpr Anchors()
	{
	}

	constexpr Anchors(float UniformAnchors)
		: Minimum(UniformAnchors)
		, Maximum(UniformAnchors)
	{
	}

	constexpr Anchors(float Horizontal, float Vertical)
		: Minimum(Horizontal, Vertical)
		, Maximum(Horizontal, Vertical)
	{
	}

	constexpr Anchors(float MinX, float MinY, float MaxX, float MaxY)
		: Minimum(MinX, MinY)
		, Maximum(MaxX, MaxY)
	{
	}

	constexpr Anchors(const Vector2& Minimum, const Vector2& Maximum)
		: Minimum(Minimum)
		, Maximum(Maximum)
	{
	}

	constexpr bool IsStretchedVertical() const
	{
		return Minimum.Y != Maximum.Y;
	}

	constexpr bool IsStretchedHorizontal() const
	{
		return Minimum.X != Maximum.X;
	}

	constexpr bool operator ==(const Anchors& Rhs) const
	{
		return Minimum == Rhs.Minimum
			&& Maximum == Rhs.Maximum;
	}

	constexpr bool operator !=(const Anchors& Rhs) const
	{
		return Minimum != Rhs.Minimum
			|| Maximum != Rhs.Maximum;
	}
};