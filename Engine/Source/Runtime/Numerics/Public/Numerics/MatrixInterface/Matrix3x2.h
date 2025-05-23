// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/MatrixInterface/Matrix2x2.h"

namespace Ayla
{
	template<class T>
	struct Matrix3x2
	{
		union
		{
			struct
			{
				T _11, _12;
				T _21, _22;
				T _31, _32;
			};
			Vector<T, 2> V[3];
		};

		constexpr Matrix3x2(T _11, T _12, T _21, T _22, T _31, T _32)
			: V
			{
				{ _11, _12 },
				{ _21, _22 },
				{ _31, _32 }
			}
		{
		}

		template<TIsVector<T, 2> IVector0, TIsVector<T, 2> IVector1, TIsVector<T, 2> IVector2>
		constexpr Matrix3x2(const IVector0& V0, const IVector1& V1, const IVector2& V2)
			: V{ V0, V1, V2 }
		{
		}

		constexpr Matrix3x2(const Matrix3x2& Rhs)
			: V{ Rhs.V[0], Rhs.V[1], Rhs.V[2] }
		{
		}

		template<TIsMatrix<T, 3, 2> IMatrix>
		constexpr Matrix3x2(const IMatrix& Rhs)
			: V{ Rhs[0], Rhs[1], Rhs[2] }
		{
		}

		constexpr Matrix3x2& operator =(const Matrix3x2& M)
		{
			V[0] = M[0];
			V[1] = M[1];
			V[2] = M[2];
			return *this;
		}

	public:
		using Type = T;
		using VectorType = Vector<T, 2>;

		constexpr Matrix3x2(T S = 0)
			: V{ Vector<T, 2>(S), Vector<T, 2>(S), Vector<T, 2>(S) }
		{
		}

		static constexpr size_t Row()
		{
			return 3;
		}

		static constexpr size_t Column()
		{
			return 2;
		}

		constexpr Matrix3x2 operator -() const
		{
			return Matrix3x2(-V[0], -V[1], -V[2]);
		}

		constexpr const VectorType& operator [](size_t N) const
		{
			return V[N];
		}

		constexpr VectorType& operator [](size_t N)
		{
			return V[N];
		}

		template<TIsMatrix<T, 3, 2> IMatrix>
		constexpr Matrix3x2& operator =(const IMatrix& M)
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

		template<TIsMatrix<T, 3, 2> IMatrix>
		inline constexpr bool NearlyEquals(const IMatrix& M, T Epsilon) const
		{
			return Matrix<>::NearlyEquals(*this, M, Epsilon);
		}

		template<TIsMatrix<T, 3, 2> IMatrix = Matrix3x2, TIsVector<T, 2> ITranslation>
		static constexpr IMatrix Translation(const ITranslation& T)
		{
			IMatrix R = IMatrix::Identity();
			R[2][0] = T[0];
			R[2][1] = T[1];
			return R;
		}

		template<TIsMatrix<T, 2, 2> IMatrix = Matrix3x2, TIsVector<T, 2> IScale>
		static constexpr IMatrix Scale(const IScale& S)
		{
			IMatrix R = IMatrix::Identity();
			R[0][0] = S[0];
			R[1][1] = S[1];
			return R;
		}

		template<TIsMatrix<T, 2, 2> IMatrix = Matrix3x2>
		static IMatrix Rotation(const Radians& Rad)
		{
			T S, C;
			Math::SinCos(Rad.Value, S, C);

			IMatrix R = IMatrix::Identity();
			R[0][0] = C;
			R[0][1] = -S;
			R[1][0] = S;
			R[1][1] = C;
			return R;
		}

		template<TIsMatrix<T, 2, 2> IMatrix = Matrix3x2, TIsVector<T, 2> IComplex>
		static constexpr IMatrix Rotation(const IComplex& C)
		{
			IMatrix R = IMatrix::Identity();
			R[0][0] = C[0];
			R[0][1] = C[1];
			R[1][0] = -C[1];
			R[1][1] = C[0];
			return R;
		}

		template<TIsMatrix<T, 2, 2> IMatrix = Matrix3x2, TIsVector<T, 2> IShear>
		static constexpr IMatrix Shear(const IShear& S)
		{
			IMatrix R = IMatrix::Identity();
			R[0][1] = S[1];
			R[1][0] = S[0];
			return R;
		}

		template<TIsMatrix<T, 3, 2> IMatrix, TIsVector<T, 2> IPoint>
		static constexpr IPoint TransformPoint(const IMatrix& M, const IPoint& P)
		{
			IPoint R;
			R[0] = P[0] * M[0][0] + P[1] * M[1][0] + M[2][0];
			R[1] = P[0] * M[0][1] + P[1] * M[1][1] + M[2][1];
			return R;
		}

		template<TIsMatrix<T, 3, 2> IMatrix, TIsVector<T, 2> IVector>
		static constexpr IVector TransformVector(const IMatrix& M, const IVector& V)
		{
			IVector R;
			R[0] = V[0] * M[0][0] + V[1] * M[1][0];
			R[1] = V[0] * M[0][1] + V[1] * M[1][1];
			return R;
		}

		template<TIsMatrix<T, 3, 2> IMatrix, TIsVector<T, 2> ITranslate, TIsVector<T, 2> IScale, TIsVector<T, 2> IComplex>
		static constexpr bool Decompose(const IMatrix& M, ITranslate& OutT, IScale& OutS, IComplex& OutR)
		{
			// Translation is just simply!
			OutT = M[2];

			// Extract scale.
			OutS = Vector2(Vector<>::Length(M[0]), Vector<>::Length(M[1]));
			if (Math::Abs(OutS[0]) < 0.001 || Math::Abs(OutS[1]) < 0.001)
			{
				return false;
			}

			// Extract rotation.
			T CosT = M[0][0] / OutS[0];
			T SinT = M[1][0];
			OutR = Vector2(CosT, SinT);
			return true;
		}

		template<TIsVector<T, 2> ITranslate, TIsVector<T, 2> IScale, TIsVector<T, 2> IComplex>
		constexpr bool Decompose(ITranslate& OutT, IScale& OutS, IComplex& OutR) const
		{
			return Decompose(*this, OutT, OutS, OutR);
		}

		template<TIsMatrix<T, 3, 2> IMatrix = Matrix3x2, TIsVector<T, 2> ITranslate, TIsVector<T, 2> IScale, TIsVector<T, 2> IComplex>
		static constexpr IMatrix AffineTransformation(const ITranslate& T, const IScale& S, const IComplex& C)
		{
			auto SM = Scale<Matrix2x2>(S);
			auto RM = Rotation<Matrix2x2>(C);

			Matrix2x2 M = SM ^ RM;
			IMatrix R = IMatrix::Identity();

			if constexpr (IMatrix::Column() == 2)
			{
				R[0] = M[0];
				R[1] = M[1];
				R[2] = T;
			}
			else
			{
				R[0][0] = M[0][0];
				R[0][1] = M[0][1];
				R[1][0] = M[1][0];
				R[1][1] = M[1][1];
				R[2][0] = T[0];
				R[2][1] = T[1];
			}
			return R;
		}

	public:
		template<TIsMatrix<T, 3, 2> IMatrix>
		static constexpr Matrix3x2 Inverse(const IMatrix& M)
		{
			T A = M[0][0], B = M[0][1], C = M[1][0], D = M[1][1];
			T Det = A * D - B * C;
			T InvDet = 1.0 / Det;
			return
			{
				D * InvDet, -B * InvDet,
				-C * InvDet, A * InvDet,
				-M[2][0], -M[2][1],
			};
		}

		constexpr Matrix3x2 Inverse() const
		{
			return Inverse(*this);
		}

		template<TIsMatrix<T, 3, 2> IMatrixL, TIsMatrix<T, 3, 2> IMatrixR>
		static constexpr Matrix3x2 Concatenate(const IMatrixL& ML, const IMatrixR& MR)
		{
			Matrix<T, 3, 3> ML33 = ToMatrix3x3(ML);
			Matrix<T, 3, 3> MR33 = ToMatrix3x3(MR);

			return Matrix<>::Multiply(ML33, MR33);
		}

		template<TIsMatrix<T, 3, 2> IMatrix>
		constexpr Matrix3x2 Concatenate(const IMatrix& M) const
		{
			return Concatenate(*this, M);
		}

		static constexpr Matrix3x2 Identity()
		{
			return Matrix3x2
			{
				1.0, 0.0,
				0.0, 1.0,
				0.0, 0.0
			};
		}

		template<TIsVector<T, 2> IVector>
		constexpr IVector TransformVector(const IVector& V) const
		{
			return TransformVector(*this, V);
		}

		template<TIsVector<T, 2> IVector>
		constexpr IVector TransformPoint(const IVector& V) const
		{
			return TransformPoint(*this, V);
		}

	private:
		template<TIsMatrix<T, 3, 2> IMatrix>
		static constexpr Matrix<T, 3, 3> ToMatrix3x3(const IMatrix& M)
		{
			Matrix<T, 3, 3> M33;
			if constexpr (IMatrix::Column() >= 3)
			{
				M33[0] = M[0];
				M33[1] = M[1];
				M33[2] = M[2];
			}
			else
			{
				M33[0] = Vector3(M[0], 0);
				M33[1] = Vector3(M[1], 0);
				M33[2] = Vector3(M[2], 0);
			}
			return M33;
		}
	};

	using Matrix3x2F = Matrix3x2<float>;
	using Matrix3x2D = Matrix3x2<double>;
}