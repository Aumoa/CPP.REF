// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<size_t N = 0>
	struct Sphere
	{
		Vector<float, N> Center;
		float Radius;

		constexpr Sphere(const Vector<float, N>& Center, float Radius = 0) : Center(Center), Radius(Radius)
		{
		}

		constexpr Sphere(const Sphere& Rhs) : Center(Rhs.Center), Radius(Rhs.Radius)
		{
		}

		template<TIsVector<float, N + 1> IVector>
		constexpr Sphere(const IVector& Rhs) : Center(Rhs), Radius(Rhs[N])
		{
		}

	public:
		using Type = float;

		constexpr Sphere(float S = 0) : Center(S), Radius(S)
		{
		}

		static constexpr size_t Size()
		{
			return N + 1;
		}

		constexpr Sphere operator -() const
		{
			return Sphere(-Center, -Radius);
		}

		constexpr const float& operator [](size_t Index) const
		{
			if (Index < N)
			{
				return Center[Index];
			}
			else
			{
				return Radius;
			}
		}

		constexpr float& operator [](size_t Index)
		{
			if (Index < N)
			{
				return Center[Index];
			}
			else
			{
				return Radius;
			}
		}

		template<TIsVector<float, N + 1> IVector>
		constexpr Sphere& operator =(const IVector& V)
		{
			Center = V;
			Radius = V[N];
			return *this;
		}

	public:
		String ToString() const;
	};

	template<>
	struct Sphere<0>
	{
		template<TIsVectorTyped<float> IVector>
		static String ToString(const IVector& S)
		{
			auto Center = Vector<>::Minor(S, S.Size() - 1);
			auto Radius = S[S.Size() - 1];
			return String::Format(TEXT("Center: {}, Radius: {}"), Center, Radius);
		}
	};

	template<size_t N>
	String Sphere<N>::ToString() const
	{
		return Sphere<>::ToString(*this);
	}
}