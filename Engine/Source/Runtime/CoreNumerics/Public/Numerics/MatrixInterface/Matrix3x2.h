// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Matrix.h"
#include "Mathematics/Radians.h"
#include "Numerics/VectorInterface/Vector.h"

struct Matrix3x2
{
	union
	{
		struct
		{
			float _11, _12;
			float _21, _22;
			float _31, _32;
		};
		Vector<float, 2> V[3];
	};

	constexpr Matrix3x2(float _11, float _12, float _21, float _22, float _31, float _32)
		: V
		{
			{ _11, _12 },
			{ _21, _22 },
			{ _31, _32 }
		}
	{
	}

	template<TIsVector<float, 2> IVector0, TIsVector<float, 2> IVector1, TIsVector<float, 2> IVector2>
	constexpr Matrix3x2(const IVector0& V0, const IVector1& V1, const IVector2& V2)
		: V{ V0, V1, V2 }
	{
	}

	constexpr Matrix3x2(const Matrix3x2& Rhs)
		: V{ Rhs.V[0], Rhs.V[1], Rhs.V[2] }
	{
	}

	template<TIsMatrix<float, 3, 2> IMatrix>
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
	using Type = float;
	using VectorType = Vector<float, 2>;

	constexpr Matrix3x2(float S = 0)
		: V{ Vector<float, 2>(S), Vector<float, 2>(S), Vector<float, 2>(S) }
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

	template<TIsMatrix<float, Row(), Column()> IMatrix>
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
	inline std::wstring ToString(std::wstring_view FormatArgs) const
	{
		return Matrix<>::ToString(*this, FormatArgs);
	}

	template<TIsMatrix<float, Row(), Column()> IMatrix>
	inline constexpr bool NearlyEquals(const IMatrix& M, float Epsilon) const
	{
		return Matrix<>::NearlyEquals(*this, M, Epsilon);
	}

	static constexpr Matrix3x2 Identity()
	{
		return Matrix3x2
		{
			1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 0.0f
		};
	}

	template<TIsMatrix<float, 3, 2> IMatrix = Matrix3x2, TIsVector<float, Column()> ITranslation>
	static constexpr IMatrix Translation(const ITranslation& T)
	{
		IMatrix R = IMatrix::Identity();
		R[2][0] = T[0];
		R[2][1] = T[1];
		return R;
	}

	template<TIsMatrix<float, 2, 2> IMatrix = Matrix3x2, TIsVector<float, Column()> IScale>
	static constexpr IMatrix Scale(const IScale& S)
	{
		IMatrix R = IMatrix::Identity();
		R[0][0] = S[0];
		R[1][1] = S[1];
		return R;
	}

	template<TIsMatrix<float, 2, 2> IMatrix = Matrix3x2>
	static IMatrix Rotation(const Radians& Rad)
	{
		float S, C;
		MathEx::SinCos(Rad.Value, S, C);

		IMatrix R = IMatrix::Identity();
		R[0][0] = C;
		R[0][1] = -S;
		R[1][0] = S;
		R[1][1] = C;
		return R;
	}

	template<TIsMatrix<float, 2, 2> IMatrix = Matrix3x2, TIsVector<float, 2> IComplex>
	static constexpr IMatrix Rotation(const IComplex& C)
	{
		IMatrix R = IMatrix::Identity();
		R[0][0] = C[0];
		R[0][1] = C[1];
		R[1][0] = -C[1];
		R[1][1] = C[0];
		return R;
	}

	template<TIsMatrix<float, 2, 2> IMatrix = Matrix3x2, TIsVector<float, Column()> IShear>
	static constexpr IMatrix Shear(const IShear& S)
	{
		IMatrix R = IMatrix::Identity();
		R[0][1] = S[1];
		R[1][0] = S[0];
		return R;
	}

	template<TIsMatrix<float, Row(), Column()> IMatrix, TIsVector<float, Column()> IPoint>
	static constexpr Vector2 TransformPoint(const IMatrix& M, const IPoint& P)
	{
		return Vector2
		{
			P[0] * M[0][0] + P[1] * M[1][0] + M[2][0],
			P[0] * M[0][1] + P[1] * M[1][1] + M[2][1]
		};
	}

	template<TIsMatrix<float, Row(), Column()> IMatrix, TIsVector<float, Column()> IVector>
	static constexpr Vector2 TransformVector(const IMatrix& M, const IVector& V)
	{
		return Vector2
		{
			V[0] * M[0][0] + V[1] * M[1][0],
			V[0] * M[0][1] + V[1] * M[1][1]
		};
	}

	template<TIsMatrix<float, Row(), Column()> IMatrix>
	static constexpr Matrix3x2 Inverse(const IMatrix& M)
	{
		float A = M[0][0], B = M[0][1], C = M[1][0], D = M[1][1];
		float Det = A * D - B * C;
		float InvDet = 1.0f / Det;
		return
		{
			D * InvDet, -B * InvDet,
			-C * InvDet, A * InvDet,
			-M[2][0], -M[2][1],
		};
	}

	template<TIsMatrix<float, 3, 2> IMatrix, TIsVector<float, 2> ITranslate, TIsVector<float, 2> IScale, TIsVector<float, 2> IComplex>
	static constexpr bool Decompose(const IMatrix& M, ITranslate& OutT, IScale& OutS, IComplex& OutR)
	{
		// Translation is just simply!
		OutT = M[2];

		// Extract scale.
		OutS = Vector2(Vector<>::Length(M[0]), Vector<>::Length(M[1]));
		if (MathEx::Abs(OutS[0]) < 0.001f || MathEx::Abs(OutS[1]) < 0.001f)
		{
			return false;
		}

		// Extract rotation.
		float CosT = M[0][0] / OutS[0];
		float SinT = M[1][0];
		OutR = Vector2(CosT, SinT);
		return true;
	}

	template<TIsVector<float, 2> ITranslate, TIsVector<float, 2> IScale, TIsVector<float, 2> IComplex>
	constexpr bool Decompose(ITranslate& OutT, IScale& OutS, IComplex& OutR) const
	{
		return Decompose(*this, OutT, OutS, OutR);
	}

	template<TIsMatrix<float, 3, 2> IMatrix = Matrix3x2, TIsVector<float, 2> ITranslate, TIsVector<float, 2> IScale, TIsVector<float, 2> IComplex>
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
};