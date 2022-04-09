// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/MatrixInterface/Matrix2x2.h"

namespace libty::inline CoreNumerics
{
	struct Shear2D
	{
		float X;
		float Y;

		constexpr Shear2D(float X, float Y) : X(X), Y(Y)
		{
		}

		constexpr Shear2D(const Shear2D& S) : X(S.X), Y(S.Y)
		{
		}

		template<TIsVector<float, 2> IShear>
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
		using Type = float;

		constexpr Shear2D(float Uniform = 0) : X(Uniform), Y(Uniform)
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

		constexpr const float& operator [](size_t N) const
		{
			return N == 0 ? X : Y;
		}

		constexpr float& operator [](size_t N)
		{
			return N == 0 ? X : Y;
		}

		template<TIsVector<float, 2> IVector>
		constexpr Shear2D& operator =(const IVector& V)
		{
			X = V[0];
			Y = V[1];
			return *this;
		}

	public:
		template<TIsMatrix<float, 2, 2> IMatrix = Matrix2x2, TIsVector<float, 2> IShear>
		static constexpr IMatrix Inverse(const IShear& S)
		{
			float InvDet = 1.0f / (1.0f - S[0] * S[1]);
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

		template<TIsMatrix<float, 2, 2> IMatrix = Matrix2x2>
		constexpr IMatrix Inverse() const
		{
			return Inverse<IMatrix>(*this);
		}

		template<TIsMatrix<float, 2, 2> IMatrix = Matrix2x2, TIsVector<float, 2> IShearL, TIsVector<float, 2> IShearR>
		static constexpr IMatrix Concatenate(const IShearL& SL, const IShearR& SR)
		{
			float XXA = SL[0];
			float YYA = SL[1];
			float XXB = SR[0];
			float YYB = SR[1];

			IMatrix R = IMatrix::Identity();
			if constexpr (IMatrix::Column() == 2)
			{
				R[0] = Vector2(1.0f + XXA * XXB, YYB * YYA);
				R[1] = Vector2(XXA + XXB, XXA * XXB + 1.0f);
			}
			else
			{
				R[0][0] = 1.0f + YYA * YYB;
				R[0][1] = YYB * YYA;
				R[1][0] = XXA + XXB;
				R[1][1] = XXA * XXB + 1.0f;
			}
			return R;
		}

		template<TIsMatrix<float, 2, 2> IMatrix = Matrix2x2, TIsVector<float, 2> IShear>
		constexpr IMatrix Concatenate(const IShear& S) const
		{
			return Concatenate(*this, S);
		}

		static constexpr Shear2D Identity()
		{
			return Shear2D(0.0f);
		}

		template<TIsVector<float, 2> IShear, TIsVector<float, 2> IPoint>
		static constexpr IPoint TransformPoint(const IShear& S, const IPoint& P)
		{
			IPoint R;
			R[0] = P[0] + P[1] * S[0];
			R[1] = P[1] + P[0] * S[1];
			return R;
		}

		template<TIsVector<float, 2> IPoint>
		constexpr IPoint TransformPoint(const IPoint& P) const
		{
			return TransformPoint(*this, P);
		}

		template<TIsVector<float, 2> IShear, TIsVector<float, 2> IVector>
		static constexpr IVector TransformVector(const IShear& S, const IVector& V)
		{
			return TransformPoint(S, V);
		}

		template<TIsVector<float, 2> IVector>
		constexpr IVector TransformVector(const IVector& V) const
		{
			return TransformVector(*this, V);
		}

	public:
		std::wstring ToString(std::wstring_view FormatArgs) const
		{
			return Vector<>::ToString(*this, FormatArgs);
		}

		constexpr bool NearlyEquals(const Scale2D& S, float Epsilon) const
		{
			return Vector<>::NearlyEquals(*this, S, Epsilon);
		}
	};
}