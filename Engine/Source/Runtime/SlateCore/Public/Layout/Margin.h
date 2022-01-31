// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
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

	constexpr Margin(float Left, float Top, float Right, float Bottom)
		: Left(Left)
		, Top(Top)
		, Right(Right)
		, Bottom(Bottom)
	{
	}

	constexpr Margin(float UniformValue)
		: Left(UniformValue)
		, Top(UniformValue)
		, Right(UniformValue)
		, Bottom(UniformValue)
	{
	}

	constexpr Margin(float Horizontal, float Vertical)
		: Left(Horizontal)
		, Top(Vertical)
		, Right(Horizontal)
		, Bottom(Vertical)
	{
	}

	constexpr bool operator ==(const Margin& Rhs) const
	{
		return Left == Rhs.Left && Top == Rhs.Top && Right == Rhs.Right && Bottom == Rhs.Bottom;
	}

	constexpr bool operator !=(const Margin& Rhs) const
	{
		return Left != Rhs.Left || Top != Rhs.Top || Right != Rhs.Right || Bottom != Rhs.Bottom;
	}

	constexpr float GetTotalSpaceAlong(EOrientation Orientation) const
	{
		switch (Orientation)
		{
		case EOrientation::Horizontal: return Left + Right;
		case EOrientation::Vertical: return Top + Bottom;
		default:
			checkf(false, L"Invalid argument: Orientation({})", (int32)Orientation);
			return 0;
		}
	}

	constexpr Vector2 GetTotalSpaceAlong() const
	{
		return Vector2(GetTotalSpaceAlong(EOrientation::Horizontal), GetTotalSpaceAlong(EOrientation::Vertical));
	}
};