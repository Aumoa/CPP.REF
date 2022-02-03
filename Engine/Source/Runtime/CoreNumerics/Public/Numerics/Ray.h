// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Vector.h"
#include <optional>

template<size_t N = 0>
struct Ray
{
	using VectorType = Vector<float, N>;

	VectorType Origin;
	VectorType Direction;
	std::optional<float> Distance;

	constexpr Ray()
	{
	}

	constexpr Ray(const VectorType& Origin, const VectorType& Dir, std::optional<float> Distance = std::nullopt)
		: Origin(Origin)
		, Direction(Dir)
		, Distance(Distance)
	{
	}

	constexpr Ray(const Ray& R)
		: Origin(R.Origin)
		, Direction(R.Direction)
		, Distance(R.Distance)
	{
	}

	constexpr Ray& operator =(const Ray& R)
	{
		Origin = R.Origin;
		Direction = R.Direction;
		Distance = R.Distance;
		return *this;
	}

	constexpr auto operator <=>(const Ray& R) const = default;
};

template<>
struct Ray<0>
{
	template<size_t N>
	static bool NearlyEquals(const Ray<N>& RL, const Ray<N>& RR, float Epsilon)
	{
		return RL.Origin.NearlyEquals(RR.Origin, Epsilon)
			&& RL.Direction.NearlyEquals(RR.Direction, Epsilon)
			&& MathEx::Abs(RL.Distance.value_or(-1.0f) - RR.Distance.value_or(-1.0f)) <= Epsilon;
	}

	template<size_t N>
	static std::wstring ToString(const Ray<N>& R, std::wstring_view FormatArgs = L"")
	{
		return std::format(L"Origin: {}, Direction: {}", R.Origin.ToString(FormatArgs), R.Direction.ToString(FormatArgs));
	}
};