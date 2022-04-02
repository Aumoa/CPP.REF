// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/MatrixInterface/Matrix3x2.h"
#include "Numerics/MatrixInterface/Matrix2x2.h"

namespace libty::inline CoreNumerics::inline TransformInterface
{
	struct Complex
	{
		float X;
		float Y;

	public:
		constexpr Complex(float X, float Y) : X(X), Y(Y)
		{
		}

		constexpr Complex(const Complex& Rhs) : X(Rhs.X), Y(Rhs.Y)
		{
		}

		template<TIsVector<float, 2> IVector>
		constexpr Complex(const IVector& V) : X(V[0]), Y(V[1])
		{
		}

		Complex& operator =(const Complex& C)
		{
			X = C.X;
			Y = C.Y;
			return *this;
		}

		template<TIsVector<float, 2> IComplex, TIsVector<float, 2> IVector>
		static constexpr IVector RotateVector(const IComplex& C, const IVector& V)
		{
			IVector R;
			R[0] = V[0] * C[0] - V[1] * C[1];
			R[1] = V[0] * C[1] + V[1] * C[0];
			return R;
		}

	public:
		using Type = float;

		constexpr Complex(float S = 0) : X(S), Y(S)
		{
		}

		static constexpr size_t Size()
		{
			return 2;
		}

		constexpr Complex operator -() const
		{
			return Complex(-X, -Y);
		}

		constexpr const float& operator [](size_t N) const
		{
			switch (N)
			{
			case 0:
				return X;
			case 1:
			default:
				return Y;
			}
		}

		constexpr float& operator [](size_t N)
		{
			switch (N)
			{
			case 0:
				return X;
			case 1:
			default:
				return Y;
			}
		}

		template<TIsVector<float, 2> IVector>
		constexpr Complex& operator =(const IVector& V)
		{
			X = V[0];
			Y = V[1];
			return *this;
		}

	public:
		template<TIsVector<float, 2> IComplex>
		static constexpr Complex Inverse(const IComplex& C)
		{
			return Complex(C[0], -C[1]);
		}

		constexpr Complex Inverse() const
		{
			return Inverse(*this);
		}

		template<TIsVector<float, 2> IComplexL, TIsVector<float, 2> IComplexR>
		static constexpr Complex Concatenate(const IComplexL& CL, const IComplexR& CR)
		{
			return TransformPoint(CL, CR);
		}

		template<TIsVector<float, 2> IComplex>
		constexpr Complex Concatenate(const IComplex& C) const
		{
			return Concatenate(*this, C);
		}

		static constexpr Complex Identity()
		{
			return Complex(1.0f, 0.0f);
		}

		template<TIsVector<float, 2> IComplex, TIsVector<float, 2> IVector>
		static constexpr IVector TransformPoint(const IComplex& C, const IVector& V)
		{
			return RotateVector(C, V);
		}

		template<TIsVector<float, 2> IVector>
		constexpr IVector TransformPoint(const IVector& V) const
		{
			return TransformPoint(*this, V);
		}

		template<TIsVector<float, 2> IComplex, TIsVector<float, 2> IVector>
		static constexpr IVector TransformVector(const IComplex& C, const IVector& V)
		{
			return RotateVector(C, V);
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

		constexpr bool NearlyEquals(const Complex& C, float Epsilon) const
		{
			return Vector<>::NearlyEquals(*this, C, Epsilon);
		}

		template<TIsMatrix<float, 2, 2> IMatrix = Matrix3x2, TIsVector<float, 2> IComplex>
		static constexpr IMatrix GetMatrix(const IComplex& C)
		{
			IMatrix M = IMatrix::Identity();
			M[0][0] = C[0];
			M[0][1] = C[1];
			M[1][0] = -C[1];
			M[1][1] = C[0];
			return M;
		}

		template<TIsMatrix<float, 2, 2> IMatrix = Matrix3x2>
		constexpr Matrix2x2 GetMatrix() const
		{
			return GetMatrix<IMatrix>(*this);
		}

		template<TIsVector<float, 2> IComplexL, TIsVector<float, 2> IComplexR>
		static constexpr Complex Multiply(const IComplexL& CL, const IComplexR& CR)
		{
			return Complex
			{
				CL[0] * CR[0] - CL[1] * CR[1],
				CL[0] * CR[1] + CL[1] * CR[0]
			};
		}

		template<TIsVector<float, 2> IComplex>
		constexpr Complex Multiply(const IComplex& C) const
		{
			return Multiply(*this, C);
		}

		static Complex Rotation(Radians R)
		{
			return Complex
			{
				MathEx::Cos(R),
				MathEx::Sin(R)
			};
		}
	};
}