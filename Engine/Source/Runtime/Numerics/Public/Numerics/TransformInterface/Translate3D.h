// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<class T>
	struct Translate3D
	{
		T X;
		T Y;
		T Z;

		constexpr Translate3D(T X, T Y, T Z) : X(X), Y(Y), Z(Z)
		{
		}

		constexpr Translate3D(const Translate3D& T) : X(T.X), Y(T.Y), Z(T.Z)
		{
		}

		template<TIsVector<T, 3> IVector>
		constexpr Translate3D(const IVector& V) : X(V[0]), Y(V[1]), Z(V[2])
		{
		}

		constexpr Translate3D& operator =(const Translate3D& T)
		{
			X = T.X;
			Y = T.Y;
			Z = T.Z;
			return *this;
		}

	public:
		using Type = T;

		constexpr Translate3D(T Uniform = 0) : X(Uniform), Y(Uniform), Z(Uniform)
		{
		}

		static constexpr size_t Size()
		{
			return 3;
		}

		constexpr Translate3D operator -() const
		{
			return Translate3D(-X, -Y, -Z);
		}

		constexpr const T& operator [](size_t N) const
		{
			switch (N)
			{
			case 0:
				return X;
			case 1:
				return Y;
			case 2:
			default:
				return Z;
			}
		}

		constexpr T& operator [](size_t N)
		{
			switch (N)
			{
			case 0:
				return X;
			case 1:
				return Y;
			case 2:
			default:
				return Z;
			}
		}

		template<TIsVector<T, 3> IVector>
		constexpr Translate3D& operator =(const IVector& V)
		{
			X = V[0];
			Y = V[1];
			Z = V[2];
			return *this;
		}

	public:
		template<TIsVector<T, 3> ITranslate>
		static constexpr Translate3D Inverse(const ITranslate& T)
		{
			return -T;
		}

		constexpr Translate3D Inverse() const
		{
			return Inverse(*this);
		}

		template<TIsVector<T, 3> ITranslateL, TIsVector<T, 3> ITranslateR>
		static constexpr Translate3D Concatenate(const ITranslateL& TL, const ITranslateR& TR)
		{
			return TL + TR;
		}

		template<TIsVector<T, 3> ITranslate>
		constexpr Translate3D Concatenate(const ITranslate& T) const
		{
			return Concatenate(*this, T);
		}

		static constexpr Translate3D Identity()
		{
			return Translate3D(0);
		}

		template<TIsVector<T, 3> ITranslate, TIsVector<T, 3> IVector>
		constexpr static IVector TransformPoint(const ITranslate& T, const IVector& P)
		{
			return P + T;
		}

		template<TIsVector<T, 3> IVector>
		constexpr IVector TransformPoint(const IVector& P) const
		{
			return TransformPoint(*this, P);
		}

		template<TIsVector<T, 3> ITranslate, TIsVector<T, 3> IVector>
		constexpr static IVector TransformVector(const ITranslate& T, const IVector& V)
		{
			return V;
		}

		template<TIsVector<T, 3> IVector>
		constexpr IVector TransformVector(const IVector& V) const
		{
			return TransformVector(*this, V);
		}

	public:
		String ToString() const
		{
			return Vector<>::ToString(*this);
		}

		constexpr bool NearlyEquals(const Translate3D& t, const T& Epsilon) const
		{
			return Vector<>::NearlyEquals(*this, t, Epsilon);
		}
	};

	using Translate3DF = Translate3D<float>;
	using Translate3DD = Translate3D<double>;
}