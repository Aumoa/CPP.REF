// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<class T>
	struct Scale3D
	{
		using IsScale = int;

		T X;
		T Y;
		T Z;

		constexpr Scale3D(T X, T Y, T Z) : X(X), Y(Y), Z(Z)
		{
		}

		constexpr Scale3D(const Scale3D& S) : X(S.X), Y(S.Y), Z(S.Z)
		{
		}

		template<TIsVector<T, 3> IScale>
		constexpr Scale3D(const IScale& S) : X(S[0]), Y(S[1]), Z(S[2])
		{
		}

		constexpr Scale3D& operator =(const Scale3D& S)
		{
			X = S.X;
			Y = S.Y;
			Z = S.Z;
			return *this;
		}

	public:
		using Type = T;

		constexpr Scale3D(T Uniform = 0) : X(Uniform), Y(Uniform), Z(Uniform)
		{
		}

		static constexpr size_t Size()
		{
			return 3;
		}

		constexpr Scale3D operator -() const
		{
			return Scale3D(-X, -Y, -Z);
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
		constexpr Scale3D& operator =(const IVector& V)
		{
			X = V[0];
			Y = V[1];
			Z = V[2];
			return *this;
		}

	public:
		template<TIsVector<T, 3> IScale>
		static constexpr Scale3D Inverse(const IScale& S)
		{
			return 1.0 / S;
		}

		constexpr Scale3D Inverse() const
		{
			return Inverse(*this);
		}

		template<TIsVector<T, 3> IScaleL, TIsVector<T, 3> IScaleR>
		static constexpr Scale3D Concatenate(const IScaleL& SL, const IScaleR& SR)
		{
			return SL * SR;
		}

		template<TIsVector<T, 3> IScale>
		constexpr Scale3D Concatenate(const IScale& S) const
		{
			return Concatenate(*this, S);
		}

		static constexpr Scale3D Identity()
		{
			return Scale3D(1.0);
		}

		template<TIsVector<T, 3> IScale, TIsVector<T, 3> IPoint>
		static constexpr IPoint TransformPoint(const IScale& S, const IPoint& P)
		{
			return S * P;
		}

		template<TIsVector<T, 3> IPoint>
		constexpr IPoint TransformPoint(const IPoint& P) const
		{
			return TransformPoint(*this, P);
		}

		template<TIsVector<T, 3> IScale, TIsVector<T, 3> IVector>
		static constexpr IVector TransformVector(const IScale& S, const IVector& V)
		{
			return S * V;
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

		constexpr bool NearlyEquals(const Scale3D& S, T Epsilon) const
		{
			return Vector<>::NearlyEquals(*this, S, Epsilon);
		}
	};

	using Scale3DF = Scale3D<float>;
	using Scale3DD = Scale3D<double>;
}