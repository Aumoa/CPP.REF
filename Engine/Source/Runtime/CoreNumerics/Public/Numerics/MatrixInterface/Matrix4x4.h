// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Matrix.h"
#include "Numerics/TransformConcepts.h"
#include "Numerics/NumericConcepts.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/TransformInterface/Translate3D.h"
#include "Numerics/TransformInterface/Scale3D.h"
#include "Numerics/TransformInterface/Quaternion.h"
#include "Mathematics/Radians.h"

namespace libty::inline CoreNumerics
{
	struct Matrix4x4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			Vector<float, 4> V[4];
		};

		constexpr Matrix4x4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44)
			: V
			{
				{ _11, _12, _13, _14 },
				{ _21, _22, _23, _24 },
				{ _31, _32, _33, _34 },
				{ _41, _42, _43, _44 }
			}
		{
		}

		template<TIsVector<float, 4> IVector0, TIsVector<float, 4> IVector1, TIsVector<float, 4> IVector2, TIsVector<float, 4> IVector3>
		constexpr Matrix4x4(const IVector0& V0, const IVector1& V1, const IVector2& V2, const IVector3& V3)
			: V{ V0, V1, V2, V3 }
		{
		}

		constexpr Matrix4x4(const Matrix4x4& Rhs)
			: V{ Rhs.V[0], Rhs.V[1], Rhs.V[2], Rhs.V[3] }
		{
		}

		template<TIsMatrix<float, 4, 4> IMatrix>
		constexpr Matrix4x4(const IMatrix& Rhs)
			: V{ Rhs[0], Rhs[1], Rhs[2], Rhs[3]}
		{
		}

		constexpr Matrix4x4& operator =(const Matrix4x4& M)
		{
			V[0] = M[0];
			V[1] = M[1];
			V[2] = M[2];
			V[3] = M[3];
			return *this;
		}

	public:
		using Type = float;
		using VectorType = Vector<float, 4>;

		constexpr Matrix4x4(float S = 0)
			: V{ Vector<float, 4>(S), Vector<float, 4>(S), Vector<float, 4>(S), Vector<float, 4>(S) }
		{
		}

		static constexpr size_t Row()
		{
			return 4;
		}

		static constexpr size_t Column()
		{
			return 4;
		}

		constexpr Matrix4x4 operator -() const
		{
			return Matrix4x4(-V[0], -V[1], -V[2], -V[3]);
		}

		constexpr const VectorType& operator [](size_t N) const
		{
			return V[N];
		}

		constexpr VectorType& operator [](size_t N)
		{
			return V[N];
		}

		template<TIsMatrix<float, 4, 4> IMatrix>
		constexpr Matrix4x4& operator =(const IMatrix& M)
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
		CORENUMERICS_API static float Determinant(const Matrix4x4& M);
		CORENUMERICS_API static Matrix4x4 Inverse(const Matrix4x4& M);
		CORENUMERICS_API static bool Decompose(const Matrix4x4& M, Translate3D& OutT, Scale3D& OutS, struct Quaternion& OutQ);

		CORENUMERICS_API static Vector3 TransformPoint(const Matrix4x4& M, const Vector3& P);
		CORENUMERICS_API static Vector3 TransformVector(const Matrix4x4& M, const Vector3& V);
		CORENUMERICS_API static Matrix4x4 Multiply(const Matrix4x4& ML, const Matrix4x4& MR);

		CORENUMERICS_API static Matrix4x4 LookToLH(const Vector3& Location, const Vector3& Dir, const Vector3& Up);
		CORENUMERICS_API static Matrix4x4 PerspectiveFovLH(Radians Fov, float AspectRatio, float Near, float Far);
		CORENUMERICS_API static Matrix4x4 AffineTransformation(const Translate3D& T, const Scale3D& S, const struct Quaternion& Q);
		CORENUMERICS_API static Matrix4x4 Translation(const Vector3& T);
		CORENUMERICS_API static Matrix4x4 Scale(const Scale3D& S);
		CORENUMERICS_API static Matrix4x4 Rotation(const Vector3& Axis, Radians Angle);
		CORENUMERICS_API static Matrix4x4 RotationX(Radians Angle);
		CORENUMERICS_API static Matrix4x4 RotationY(Radians Angle);
		CORENUMERICS_API static Matrix4x4 RotationZ(Radians Angle);

		template<TIsMatrix<float, 4, 4> IMatrix>
		static auto Determinant(const IMatrix& M)
		{
			return Determinant(Matrix4x4(M));
		}

		template<TIsMatrix<float, 4, 4> IMatrix, TIsVector<float, 3> ITranslation, TIsVector<float, 3> IScale, TIsVector<float, 4> IQuaternion>
		static auto Decompose(const IMatrix& M, ITranslation& OutT, IScale& OutS, IQuaternion& OutQ)
		{
			Translate3D T;
			Scale3D S;
			Quaternion Q;

			auto R = Decompose(Matrix4x4(M), T, S, Q);
			if (R)
			{
				OutT = T;
				OutS = S;
				OutQ = Q;
			}

			return R;
		}

		template<TIsVector<float, 3> ITranslation, TIsVector<float, 3> IScale, TIsVector<float, 4> IQuaternion>
		auto Decompose(ITranslation& OutT, IScale& OutS, IQuaternion& OutQ) const
		{
			return Decompose(*this, OutT, OutS, OutQ);
		}

		template<TIsMatrix<float, 4, 4> IMatrixL, TIsMatrix<float, 4, 4> IMatrixR>
		static auto Multiply(const IMatrixL ML, const IMatrixR& MR)
		{
			return Multiply(Matrix4x4(ML), Matrix4x4(MR));
		}


		template<TIsVector<float, 3> ILocation, TIsVector<float, 3> IDir, TIsVector<float, 3> IUp>
		static auto LookToLH(const ILocation& Location, const IDir& Dir, const IUp& Up)
		{
			return LookToLH(Vector3(Location), Vector3(Dir), Vector3(Up));
		}

		template<TIsMatrix<float, 4, 3> IMatrix = Matrix4x4, TIsVector<float, 3> ITranslation, TIsVector<float, 3> IScale, TIsVector<float, 4> IQuaternion>
		static auto AffineTransformation(const ITranslation& T, const IScale& S, const IQuaternion& Q)
		{
			Matrix4x4 M = AffineTransformation(Translate3D(T), Scale3D(S), Quaternion(Q));
			IMatrix R = IMatrix::Identity();

			for (size_t i = 0; i < 4; ++i)
			{
				for (size_t j = 0; j < 3; ++j)
				{
					R[i][j] = M[i][j];
				}
			}

			return R;
		}

		template<TIsVector<float, 3> ITranslation>
		static auto Translation(const ITranslation& T)
		{
			return Translation(Vector3(T));
		}

		template<TIsVector<float, 3> IScale>
		static auto Scale(const IScale& S)
		{
			return Scale(Scale3D(S));
		}

		template<TIsVector<float, 3> IAxis>
		static auto Rotation(const IAxis& Axis, Radians Angle)
		{
			return Rotation(Vector3(Axis), Angle);
		}



		inline String ToString(String formatArgs) const
		{
			return Matrix<>::ToString(*this, formatArgs);
		}

		template<TIsMatrix<float, 4, 4> IMatrix>
		inline constexpr bool NearlyEquals(const IMatrix& M, float Epsilon) const
		{
			return Matrix<>::NearlyEquals(M, Epsilon);
		}

	public:
		template<TIsMatrix<float, 4, 4> IMatrix>
		static auto Inverse(const IMatrix& M)
		{
			return Inverse(Matrix4x4(M));
		}

		auto Inverse() const
		{
			return Inverse(*this);
		}

		inline static constexpr Matrix4x4 Identity()
		{
			return Matrix4x4
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		template<TIsMatrix<float, 4, 4> IMatrixL, TIsMatrix<float, 4, 4> IMatrixR>
		static auto Concatenate(const IMatrixL& ML, const IMatrixR& MR)
		{
			return Multiply(ML, MR);
		}

		template<TIsMatrix<float, 4, 4> IMatrix>
		auto Concatenate(const IMatrix& M) const
		{
			return Multiply(*this, M);
		}

		template<TIsMatrix<float, 4, 4> IMatrix, TIsVector<float, 3> IPoint>
		static IPoint TransformPoint(const IMatrix& M, const IPoint& P)
		{
			return TransformPoint(Matrix4x4(M), Vector3(P));
		}

		template<TIsVector<float, 3> IPoint>
		IPoint TransformPoint(const IPoint& P) const
		{
			return TransformPoint(*this, Vector3(P));
		}

		template<TIsMatrix<float, 4, 4> IMatrix, TIsVector<float, 3> IVector>
		static IVector TransformVector(const IMatrix& M, const IVector& V)
		{
			return TransformVector(Matrix4x4(M), Vector3(V));
		}

		template<TIsVector<float, 3> IVector>
		IVector TransformVector(const IVector& V) const
		{
			return TransformVector(*this, Vector3(V));
		}
	};
}