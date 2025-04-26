// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<class T = void, size_t N = 0>
	struct Ray
	{
		using VectorType = Vector<T, N>;

		VectorType Origin;
		VectorType Direction;
		std::optional<T> Distance;

		constexpr Ray()
		{
		}

		constexpr Ray(const VectorType& Origin, const VectorType& Dir, std::optional<T> Distance = std::nullopt)
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
	struct Ray<void, 0>
	{
		template<class T, size_t N>
		static bool NearlyEquals(const Ray<T, N>& RL, const Ray<T, N>& RR, T Epsilon)
		{
			return RL.Origin.NearlyEquals(RR.Origin, Epsilon)
				&& RL.Direction.NearlyEquals(RR.Direction, Epsilon)
				&& Math::Abs(RL.Distance.value_or(-1.0f) - RR.Distance.value_or(-1.0f)) <= Epsilon;
		}

		template<class T, size_t N>
		static String ToString(const Ray<T, N>& R, String FormatArgs = TEXT(""))
		{
			return String::Format(L"Origin: {}, Direction: {}", R.Origin.ToString(FormatArgs), R.Direction.ToString(FormatArgs));
		}
	};

	template<size_t N>
	using RayF = Ray<float, N>;
	template<size_t N>
	using RayD = Ray<double, N>;
}