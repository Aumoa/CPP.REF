// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Layout/Orientation.h"

struct NMargin
{
	float Left = 0;
	float Top = 0;
	float Right = 0;
	float Bottom = 0;

	constexpr NMargin()
	{
	}

	constexpr NMargin(float Left, float Top, float Right, float Bottom)
		: Left(Left)
		, Top(Top)
		, Right(Right)
		, Bottom(Bottom)
	{
	}

	constexpr NMargin(float UniformValue)
		: Left(UniformValue)
		, Top(UniformValue)
		, Right(UniformValue)
		, Bottom(UniformValue)
	{
	}

	constexpr NMargin(float Horizontal, float Vertical)
		: Left(Horizontal)
		, Top(Vertical)
		, Right(Horizontal)
		, Bottom(Vertical)
	{
	}

	constexpr NMargin(const NMargin& M)
		: Left(M.Left)
		, Top(M.Top)
		, Right(M.Right)
		, Bottom(M.Bottom)
	{
	}

	template<TIsVector<float, 4> INMargin>
	constexpr NMargin(const INMargin& M)
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
			checkf(false, TEXT("Invalid argument: Orientation({})"), (int32)Orientation);
			return 0.0f;
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

	constexpr NMargin operator -() const
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
	constexpr NMargin& operator =(const IMargin& M)
	{
		Left = M[0];
		Top = M[1];
		Right = M[2];
		Bottom = M[3];
		return *this;
	}
};