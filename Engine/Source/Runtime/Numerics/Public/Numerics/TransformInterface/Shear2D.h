// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/MatrixInterface/Matrix2x2.h"
#include "Numerics/TransformInterface/Scale2D.h"

namespace Ayla
{
	template<class T>
	struct Shear2D
	{
		T X;
		T Y;

		constexpr Shear2D(T X, T Y) : X(X), Y(Y)
		{
		}

		constexpr Shear2D(const Shear2D& S) : X(S.X), Y(S.Y)
		{
		}

		template<TIsVector<T, 2> IShear>
		constexpr Shear2D(const IShear& S) : X(S[0]), Y(S[1])
		{
		}

		constexpr Shear2D& operator =(const Shear2D& S)
		{
			X = S.X;
			Y = S.Y;
			return *this;
		}

	public:
		using Type = T;

		constexpr Shear2D(T Uniform = 0) : X(Uniform), Y(Uniform)
		{
		}

		static constexpr size_t Size()
		{
			return 2;
		}

		constexpr Shear2D operator -() const
		{
			return Shear2D(-X, -Y);
		}

		constexpr const T& operator [](size_t N) const
		{
			return N == 0 ? X : Y;
		}

		constexpr T& operator [](size_t N)
		{
			return N == 0 ? X : Y;
		}

		template<TIsVector<T, 2> IVector>
		constexpr Shear2D& operator =(const IVector& V)
		{
			X = V[0];
			Y = V[1];
			return *this;
		}

	public:
		template<TIsMatrix<T, 2, 2> IMatrix = Matrix2x2, TIsVector<T, 2> IShear>
		static constexpr IMatrix Inverse(const IShear& S)
		{
			T InvDet = 1.0 / (1.0 - S[0] * S[1]);
			IMatrix R = IMatrix::Identity();
			if constexpr (IMatrix::Column() == 2)
			{
				R[0] = Vector2(InvDet, -S[1] * InvDet);
				R[1] = Vector2(-S[0] * InvDet, InvDet);
			}
			else
			{
				R[0][0] = InvDet;
				R[0][1] = -S[1] * InvDet;
				R[1][0] = -S[0] * InvDet;
				R[1][1] = InvDet;
			}
			return R;
		}

		template<TIsMatrix<T, 2, 2> IMatrix = Matrix2x2>
		constexpr IMatrix Inverse() const
		{
			return Inverse<IMatrix>(*this);
		}

		template<TIsMatrix<T, 2, 2> IMatrix = Matrix2x2, TIsVector<T, 2> IShearL, TIsVector<T, 2> IShearR>
		static constexpr IMatrix Concatenate(const IShearL& SL, const IShearR& SR)
		{
			T XXA = SL[0];
			T YYA = SL[1];
			T XXB = SR[0];
			T YYB = SR[1];

			IMatrix R = IMatrix::Identity();
			if constexpr (IMatrix::Column() == 2)
			{
				R[0] = Vector2(1.0 + XXA * XXB, YYB * YYA);
				R[1] = Vector2(XXA + XXB, XXA * XXB + 1.0);
			}
			else
			{
				R[0][0] = 1.0 + YYA * YYB;
				R[0][1] = YYB * YYA;
				R[1][0] = XXA + XXB;
				R[1][1] = XXA * XXB + 1.0;
			}
			return R;
		}

		template<TIsMatrix<T, 2, 2> IMatrix = Matrix2x2, TIsVector<T, 2> IShear>
		constexpr IMatrix Concatenate(const IShear& S) const
		{
			return Concatenate(*this, S);
		}

		static constexpr Shear2D Identity()
		{
			return Shear2D(0.0f);
		}

		template<TIsVector<T, 2> IShear, TIsVector<T, 2> IPoint>
		static constexpr IPoint TransformPoint(const IShear& S, const IPoint& P)
		{
			IPoint R;
			R[0] = P[0] + P[1] * S[0];
			R[1] = P[1] + P[0] * S[1];
			return R;
		}

		template<TIsVector<T, 2> IPoint>
		constexpr IPoint TransformPoint(const IPoint& P) const
		{
			return TransformPoint(*this, P);
		}

		template<TIsVector<T, 2> IShear, TIsVector<T, 2> IVector>
		static constexpr IVector TransformVector(const IShear& S, const IVector& V)
		{
			return TransformPoint(S, V);
		}

		template<TIsVector<T, 2> IVector>
		constexpr IVector TransformVector(const IVector& V) const
		{
			return TransformVector(*this, V);
		}

	public:
		String ToString() const
		{
			return Vector<>::ToString(*this);
		}

		constexpr bool NearlyEquals(const Scale2D& S, T Epsilon) const
		{
			return Vector<>::NearlyEquals(*this, S, Epsilon);
		}
	};

	using Shear2DF = Shear2D<float>;
	using Shear2DD = Shear2D<double>;
}