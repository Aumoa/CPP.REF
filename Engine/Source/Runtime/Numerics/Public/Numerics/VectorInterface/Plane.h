// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	struct Plane
	{
		Vector3 Normal;
		float Distance;

		constexpr Plane(float NormalX, float NormalY, float NormalZ, float Distance)
			: Normal(NormalX, NormalY, NormalZ)
			, Distance(Distance)
		{
		}

		constexpr Plane(const Vector3 Normal, float Distance = 0) : Normal(Normal), Distance(Distance)
		{
		}

		constexpr Plane(const Plane& Rhs) : Normal(Rhs.Normal), Distance(Rhs.Distance)
		{
		}

		template<TIsVector<float, 3> INormal>
		constexpr Plane(const INormal& Normal, float Distance = 0) : Normal(Normal), Distance(Distance)
		{
		}

		template<TIsVector<float, 4> IVector>
		constexpr Plane(const IVector& V) : Normal(V), Distance(V[4])
		{
		}

		constexpr Plane& operator =(const Plane& P)
		{
			Normal = P.Normal;
			Distance = P.Distance;
			return *this;
		}

	public:
		using Type = float;

		constexpr Plane(float S = 0) : Normal(S), Distance(S)
		{
		}

		static constexpr size_t Size()
		{
			return 4;
		}

		constexpr Plane operator -() const
		{
			return Plane(-Normal, -Distance);
		}

		constexpr const float& operator [](size_t N) const
		{
			switch (N)
			{
			case 0:
			case 1:
			case 2:
				return Normal[N];
			case 3:
			default:
				return Distance;
			}
		}

		constexpr float& operator [](size_t N)
		{
			switch (N)
			{
			case 0:
			case 1:
			case 2:
				return Normal[N];
			case 3:
			default:
				return Distance;
			}
		}

		template<TIsVector<float, 4> IVector>
		constexpr Plane& operator =(const IVector& V)
		{
			Normal = { V[0], V[1], V[2] };
			Distance = V[3];
			return *this;
		}

	public:
		String ToString(String FormatArgs = TEXT("")) const
		{
			return String::Format(TEXT("Normal: {}, Distance: {}"), Vector<>::ToString(Normal), Distance);
		}

		Plane GetNormal() const
		{
			float S = Math::InvSqrt(Vector<>::LengthSq(Normal));
			return Plane(Normal * S, Distance * S);
		}

		template<TIsVector<float, 4> IPlane, TIsVector<float, 3> ICoord>
		static float DotCoord(const IPlane& Plane, const ICoord& Coord)
		{
			return Plane[0] * Coord[0] + Plane[1] * Coord[1] + Plane[2] * Coord[2] + Plane[3] * 1.0f;
		}
	};
}