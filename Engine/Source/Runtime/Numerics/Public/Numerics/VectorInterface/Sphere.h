// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<class T, size_t N>
	struct Sphere
	{
		Vector<T, N> Center;
		T Radius;

		constexpr Sphere(const Vector<T, N>& Center, T Radius = 0) : Center(Center), Radius(Radius)
		{
		}

		constexpr Sphere(const Sphere& Rhs) : Center(Rhs.Center), Radius(Rhs.Radius)
		{
		}

		template<TIsVector<T, N + 1> IVector>
		constexpr Sphere(const IVector& Rhs) : Center(Rhs), Radius(Rhs[N])
		{
		}

	public:
		using Type = T;

		constexpr Sphere(T S = 0) : Center(S), Radius(S)
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

		constexpr const T& operator [](size_t Index) const
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

		constexpr T& operator [](size_t Index)
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

		template<TIsVector<T, N + 1> IVector>
		constexpr Sphere& operator =(const IVector& V)
		{
			Center = V;
			Radius = V[N];
			return *this;
		}

	public:
		String ToString() const;
	};

	template<class T, size_t N>
	String Sphere<T, N>::ToString() const
	{
		return String::Format(TEXT("Center: {}, Radius: {}"), Center, Radius);
	}

	using Sphere2DF = Sphere<float, 2>;
	using Sphere2DD = Sphere<double, 2>;
	using Sphere3DF = Sphere<float, 3>;
	using Sphere3DD = Sphere<double, 3>;
}