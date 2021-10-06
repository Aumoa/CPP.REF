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

	constexpr Anchors(float uniformAnchors)
		: Minimum(uniformAnchors)
		, Maximum(uniformAnchors)
	{
	}

	constexpr Anchors(float horizontal, float vertical)
		: Minimum(horizontal, vertical)
		, Maximum(horizontal, vertical)
	{
	}

	constexpr Anchors(float minX, float minY, float maxX, float maxY)
		: Minimum(minX, minY)
		, Maximum(maxX, maxY)
	{
	}

	constexpr Anchors(const Vector2& minimum, const Vector2& maximum)
		: Minimum(minimum)
		, Maximum(maximum)
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

	constexpr bool operator ==(const Anchors& rhs) const
	{
		return Minimum == rhs.Minimum
			&& Maximum == rhs.Maximum;
	}

	constexpr bool operator !=(const Anchors& rhs) const
	{
		return Minimum != rhs.Minimum
			|| Maximum != rhs.Maximum;
	}
};