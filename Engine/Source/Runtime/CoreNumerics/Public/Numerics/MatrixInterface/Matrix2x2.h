// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Matrix.h"
#include "Mathematics/Radians.h"

struct Matrix2x2
{
	union
	{
		struct
		{
			float _11, _12;
			float _21, _22;
		};
		Vector<float, 2> V[2];
	};

	constexpr Matrix2x2(float _11, float _12, float _21, float _22)
		: V
		{
			{ _11, _12 },
			{ _21, _22 }
		}
	{
	}

	template<TIsVector<float, 2> IVector0, TIsVector<float, 2> IVector1>
	constexpr Matrix2x2(const IVector0& V0, const IVector1& V1)
		: V{ V0, V1 }
	{
	}

	constexpr Matrix2x2(const Matrix2x2& Rhs)
		: V{ Rhs.V[0], Rhs.V[1] }
	{
	}

	template<TIsMatrix<float, 2, 2> IMatrix>
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
	using Type = float;
	using VectorType = Vector<float, 2>;

	constexpr Matrix2x2(float S = 0)
		: V{ Vector<float, 2>(S), Vector<float, 2>(S) }
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

	template<TIsMatrix<float, Row(), Column()> IMatrix>
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
	inline std::wstring ToString(std::wstring_view FormatArgs) const
	{
		return Matrix<>::ToString(*this, FormatArgs);
	}

	template<TIsMatrix<float, Row(), Column()> IMatrix>
	inline constexpr bool NearlyEquals(const IMatrix& M, float Epsilon) const
	{
		return Matrix<>::NearlyEquals(*this, M, Epsilon);
	}

	static constexpr Matrix2x2 Identity()
	{
		return Matrix2x2
		{
			1.0f, 0.0f,
			0.0f, 1.0f,
		};
	}

	template<TIsVector<float, Column()> IScale>
	static constexpr Matrix2x2 Scale(const IScale& S)
	{
		return Matrix2x2
		{
			S[0], 0.0f,
			0.0f, S[1]
		};
	}

	static Matrix2x2 Rotation(const Radians& Rad)
	{
		float S, C;
		MathEx::SinCos(Rad.Value, S, C);

		return Matrix2x2
		{
			C, -S,
			S, C
		};
	}

	template<TIsVector<float, Column()> IShear>
	static constexpr Matrix2x2 Shear(const IShear& S)
	{
		return Matrix2x2
		{
			1.0f, S[1],
			S[0], 1.0f
		};
	}

	template<TIsMatrix<float, Row(), Column()> IMatrix, TIsVector<float, Column()> IPoint>
	static constexpr Vector2 TransformPoint(const IMatrix& M, const IPoint& P)
	{
		return Vector2
		{
			P[0] * M[0][0] + P[1] * M[1][0],
			P[0] * M[0][1] + P[1] * M[1][1]
		};
	}

	template<TIsMatrix<float, Row(), Column()> IMatrix, TIsVector<float, Column()> IVector>
	static constexpr Vector2 TransformVector(const IMatrix& M, const IVector& V)
	{
		return TransformPoint(M, V);
	}

	template<TIsMatrix<float, Row(), Column()> IMatrix>
	static constexpr Matrix2x2 Inverse(const IMatrix& M)
	{
		float A = M[0][0], B = M[0][1], C = M[1][0], D = M[1][1];
		float Det = A * D - B * C;
		float InvDet = 1.0f / Det;
		return
		{
			D * InvDet, -B * InvDet,
			-C * InvDet, A * InvDet
		};
	}
};