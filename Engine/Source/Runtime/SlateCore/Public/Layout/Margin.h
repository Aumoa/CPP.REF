// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "LayoutEnums.h"

struct Margin
{
	float Left = 0;
	float Top = 0;
	float Right = 0;
	float Bottom = 0;

	constexpr Margin()
	{
	}

	constexpr Margin(float left, float top, float right, float bottom)
		: Left(left)
		, Top(top)
		, Right(right)
		, Bottom(bottom)
	{
	}

	constexpr Margin(float uniformValue)
		: Left(uniformValue)
		, Top(uniformValue)
		, Right(uniformValue)
		, Bottom(uniformValue)
	{
	}

	constexpr Margin(float horizontal, float vertical)
		: Left(horizontal)
		, Top(vertical)
		, Right(horizontal)
		, Bottom(vertical)
	{
	}

	constexpr bool operator ==(const Margin& rhs) const
	{
		return Left == rhs.Left && Top == rhs.Top && Right == rhs.Right && Bottom == rhs.Bottom;
	}

	constexpr bool operator !=(const Margin& rhs) const
	{
		return Left != rhs.Left || Top != rhs.Top || Right != rhs.Right || Bottom != rhs.Bottom;
	}

	constexpr float GetTotalSpaceAlong(EOrientation orientation) const
	{
		switch (orientation)
		{
		case EOrientation::Horizontal: return Left + Right;
		case EOrientation::Vertical: return Top + Bottom;
		default:
			check(false);
			return 0;
		}
	}
};