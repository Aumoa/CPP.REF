// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreNumerics.h"
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

	constexpr Margin(const Margin& M)
		: Left(M.Left)
		, Top(M.Top)
		, Right(M.Right)
		, Bottom(M.Bottom)
	{
	}

	template<TIsVector<float, 4> IMargin>
	constexpr Margin(const IMargin& M)
		: Left(M[0])
		, Top(M[1])
		, Right(M[2])
		, Bottom(M[3])
	{
	}

	constexpr float GetTotalSpaceAlong(EOrientation Orientation) const
	{
		switch (Orientation)
		{
		case EOrientation::Horizontal: return Left + Right;
		case EOrientation::Vertical: return Top + Bottom;
		default:
			throw fatal_exception(String::Format("Invalid argument: Orientation({})", (int32)Orientation));
		}
	}

	constexpr Vector2 GetTotalSpaceAlong() const
	{
		return Vector2(GetTotalSpaceAlong(EOrientation::Horizontal), GetTotalSpaceAlong(EOrientation::Vertical));
	}

public:
	using Type = float;

	static constexpr size_t Size()
	{
		return 4;
	}

	constexpr Margin operator -() const
	{
		return { -Left, -Top, -Right, -Bottom };
	}

	constexpr const float& operator [](size_t N) const
	{
		switch (N)
		{
		case 0:
			return Left;
		case 1:
			return Top;
		case 2:
			return Right;
		case 3:
		default:
			return Bottom;
		}
	}

	constexpr float& operator [](size_t N)
	{
		switch (N)
		{
		case 0:
			return Left;
		case 1:
			return Top;
		case 2:
			return Right;
		case 3:
		default:
			return Bottom;
		}
	}

	template<TIsVector<float, 4> IMargin>
	constexpr Margin& operator =(const IMargin& M)
	{
		Left = M[0];
		Top = M[1];
		Right = M[2];
		Bottom = M[3];
		return *this;
	}
};