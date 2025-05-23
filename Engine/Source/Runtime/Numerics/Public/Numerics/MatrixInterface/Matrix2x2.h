// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/MatrixInterface/Matrix.h"
#include "Numerics/TransformConcepts.h"

namespace Ayla
{
	template<class T>
	struct Matrix2x2
	{
		union
		{
			struct
			{
				T _11, _12;
				T _21, _22;
			};
			Vector<T, 2> V[2];
		};

		constexpr Matrix2x2(T _11, T _12, T _21, T _22)
			: V
			{
				{ _11, _12 },
				{ _21, _22 }
			}
		{
		}

		template<TIsVector<T, 2> IVector0, TIsVector<T, 2> IVector1>
		constexpr Matrix2x2(const IVector0& V0, const IVector1& V1)
			: V{ V0, V1 }
		{
		}

		constexpr Matrix2x2(const Matrix2x2& Rhs)
			: V{ Rhs.V[0], Rhs.V[1] }
		{
		}

		template<TIsMatrix<T, 2, 2> IMatrix>
		constexpr Matrix2x2(const IMatrix& Rhs)
			: V{ Rhs[0], Rhs[1] }
		{
		}

		constexpr Matrix2x2& operator =(const Matrix2x2& M)
		{
			V[0] = M[0];
			V[1] = M[1];
			return *this;
		}

	public:
		using Type = T;
		using VectorType = Vector<T, 2>;

		constexpr Matrix2x2(T S = 0)
			: V{ Vector<T, 2>(S), Vector<T, 2>(S) }
		{
		}

		static constexpr size_t Row()
		{
			return 2;
		}

		static constexpr size_t Column()
		{
			return 2;
		}

		constexpr Matrix2x2 operator -() const
		{
			return Matrix2x2(-V[0], -V[1]);
		}

		constexpr const VectorType& operator [](size_t N) const
		{
			return V[N];
		}

		constexpr VectorType& operator [](size_t N)
		{
			return V[N];
		}

		template<TIsMatrix<T, 2, 2> IMatrix>
		constexpr Matrix2x2& operator =(const IMatrix& M)
		{
			for (size_t i = 0; i < Row(); ++i)
			{
				for (size_t j = 0; j < Column(); ++j)
				{
					V[i][j] = M[i][j];
				}
			}

			return *this;
		}

	public:
		inline String ToString() const
		{
			return Matrix<>::ToString(*this);
		}

		template<TIsMatrix<T, 2, 2> IMatrix>
		inline constexpr bool NearlyEquals(const IMatrix& M, T Epsilon) const
		{
			return Matrix<>::NearlyEquals(*this, M, Epsilon);
		}

		template<TIsVector<T, 2> IScale>
		static constexpr Matrix2x2 Scale(const IScale& S)
		{
			return Matrix2x2
			{
				S[0], 0.0,
				0.0, S[1]
			};
		}

		static Matrix2x2 Rotation(const Radians& Rad)
		{
			T S, C;
			Math::SinCos(Rad.Value, S, C);

			return Matrix2x2
			{
				C, -S,
				S, C
			};
		}

		template<TIsVector<T, 2> IShear>
		static constexpr Matrix2x2 Shear(const IShear& S)
		{
			return Matrix2x2
			{
				1.0, S[1],
				S[0], 1.0
			};
		}

	public:
		template<TIsMatrix<T, 2, 2> IMatrix>
		static constexpr Matrix2x2 Inverse(const IMatrix& M)
		{
			T A = M[0][0], B = M[0][1], C = M[1][0], D = M[1][1];
			T Det = A * D - B * C;
			T InvDet = 1.0 / Det;
			return
			{
				D * InvDet, -B * InvDet,
				-C * InvDet, A * InvDet
			};
		}

		constexpr Matrix2x2 Inverse() const
		{
			return Inverse(*this);
		}

		template<TIsMatrix<T, 2, 2> IMatrixL, TIsMatrix<T, 2, 2> IMatrixR>
		static constexpr Matrix2x2 Concatenate(const IMatrixL& ML, const IMatrixR& MR)
		{
			return Matrix<>::Multiply(ML, MR);
		}

		template<TIsMatrix<T, 2, 2> IMatrix>
		constexpr Matrix2x2 Concatenate(const IMatrix& M) const
		{
			return Concatenate(*this, M);
		}

		static constexpr Matrix2x2 Identity()
		{
			return Matrix2x2
			{
				1.0, 0.0,
				0.0, 1.0,
			};
		}

		template<TIsMatrix<T, 2, 2> IMatrix, TIsVector<T, 2> IVector>
		static constexpr IVector TransformVector(const IMatrix& M, const IVector& V)
		{
			return Matrix<>::TransformVector(M, V);
		}

		template<TIsVector<T, 2> IVector>
		constexpr IVector TransformVector(const IVector& V) const
		{
			return TransformVector(*this, V);
		}

		template<TIsMatrix<T, 2, 2> IMatrix, TIsVector<T, 2> IVector>
		static constexpr IVector TransformPoint(const IMatrix& M, const IVector& V)
		{
			return Matrix<>::TransformVector(M, V);
		}

		template<TIsVector<T, 2> IVector>
		constexpr IVector TransformPoint(const IVector& V) const
		{
			return TransformPoint(*this, V);
		}
	};

	using Matrix2x2F = Matrix2x2<float>;
	using Matrix2x2D = Matrix2x2<double>;
}