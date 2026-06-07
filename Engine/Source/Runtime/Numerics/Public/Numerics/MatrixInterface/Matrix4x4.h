// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/MatrixInterface/Matrix.h"
#include "Numerics/TransformInterface/Translate3D.h"
#include "Numerics/TransformInterface/Scale3D.h"
#include "Numerics/TransformInterface/Quaternion.h"
#include "Numerics/DirectXMath.h"

namespace Ayla
{
	template<class T = void>
	struct alignas(std::same_as<T, float> ? 16 : alignof(T)) Matrix4x4
	{
		union
		{
			Vector<T, 4> V[4];
			struct
			{
				T _11, _12, _13, _14;
				T _21, _22, _23, _24;
				T _31, _32, _33, _34;
				T _41, _42, _43, _44;
			};
		};

		constexpr Matrix4x4(T _11, T _12, T _13, T _14, T _21, T _22, T _23, T _24, T _31, T _32, T _33, T _34, T _41, T _42, T _43, T _44)
			: V
			{
				{ _11, _12, _13, _14 },
				{ _21, _22, _23, _24 },
				{ _31, _32, _33, _34 },
				{ _41, _42, _43, _44 }
			}
		{
		}

		template<TIsVector<T, 4> IVector0, TIsVector<T, 4> IVector1, TIsVector<T, 4> IVector2, TIsVector<T, 4> IVector3>
		constexpr Matrix4x4(const IVector0& V0, const IVector1& V1, const IVector2& V2, const IVector3& V3)
			: V{ V0, V1, V2, V3 }
		{
		}

		constexpr Matrix4x4(const Matrix4x4& Rhs)
			: V{ Rhs.V[0], Rhs.V[1], Rhs.V[2], Rhs.V[3] }
		{
		}

		template<TIsMatrix<T, 4, 4> IMatrix>
		constexpr Matrix4x4(const IMatrix& Rhs)
			: V{ Rhs[0], Rhs[1], Rhs[2], Rhs[3] }
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
		using Type = T;
		using VectorType = Vector<T, 4>;

		constexpr Matrix4x4(T S = 0)
			: V{ Vector<T, 4>(S), Vector<T, 4>(S), Vector<T, 4>(S), Vector<T, 4>(S) }
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

		template<TIsMatrix<T, 4, 4> IMatrix>
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
		template<TIsMatrix<T, 4, 4> IMatrix>
		static auto Determinant(const IMatrix& M)
		{
			return Determinant(Matrix4x4(M));
		}

		template<TIsMatrix<T, 4, 4> IMatrix, TIsVector<T, 3> ITranslation, TIsVector<T, 3> IScale, TIsVector<T, 4> IQuaternion>
		static auto Decompose(const IMatrix& M, ITranslation& OutT, IScale& OutS, IQuaternion& OutQ)
		{
			Translate3D<T> translate;
			Scale3D<T> scale;
			Quaternion<T> rot;

			auto R = Decompose(Matrix4x4(M), translate, scale, rot);
			if (R)
			{
				OutT = translate;
				OutS = scale;
				OutQ = rot;
			}

			return R;
		}

		template<TIsVector<T, 3> ITranslation, TIsVector<T, 3> IScale, TIsVector<T, 4> IQuaternion>
		auto Decompose(ITranslation& OutT, IScale& OutS, IQuaternion& OutQ) const
		{
			return Decompose(*this, OutT, OutS, OutQ);
		}

		template<TIsMatrix<T, 4, 4> IMatrixL, TIsMatrix<T, 4, 4> IMatrixR>
		static auto Multiply(const IMatrixL ML, const IMatrixR& MR)
		{
			return Multiply(Matrix4x4(ML), Matrix4x4(MR));
		}


		template<TIsVector<T, 3> ILocation, TIsVector<T, 3> IDir, TIsVector<T, 3> IUp>
		static Matrix4x4<T> LookToLH(const ILocation& Location, const IDir& Dir, const IUp& Up);

		template<TIsMatrix<T, 4, 3> IMatrix = Matrix4x4, TIsVector<T, 3> ITranslation, TIsVector<T, 3> IScale, TIsVector<T, 4> IQuaternion>
		static auto AffineTransformation(const ITranslation& translate, const IScale& scale, const IQuaternion& rot);

		template<TIsVector<T, 3> ITranslation>
		static auto Translation(const ITranslation& translate)
		{
			return Translation(Vector3(translate));
		}

		template<TIsVector<T, 3> IScale>
		static auto Scale(const IScale& S)
		{
			return Scale(Scale3D(S));
		}

		template<TIsVector<T, 3> IAxis>
		static auto Rotation(const IAxis& Axis, const Radians<T>& Angle)
		{
			return Rotation(Vector3(Axis), Angle);
		}

		inline String ToString() const
		{
			return Matrix<>::ToString(*this);
		}

		template<TIsMatrix<T, 4, 4> IMatrix>
		inline constexpr bool NearlyEquals(const IMatrix& M, T Epsilon) const
		{
			return Matrix<>::NearlyEquals(M, Epsilon);
		}

	public:
		template<TIsMatrix<T, 4, 4> IMatrix>
		static Matrix4x4 Inverse(const IMatrix& M)
		{
			return Inverse(Matrix4x4(M));
		}

		Matrix4x4 Inverse() const;

		inline static constexpr Matrix4x4 Identity()
		{
			return Matrix4x4
			{
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
			};
		}

		template<TIsMatrix<T, 4, 4> IMatrixL, TIsMatrix<T, 4, 4> IMatrixR>
		static auto Concatenate(const IMatrixL& ML, const IMatrixR& MR)
		{
			return Multiply(ML, MR);
		}

		template<TIsMatrix<T, 4, 4> IMatrix>
		auto Concatenate(const IMatrix& M) const
		{
			return Multiply(*this, M);
		}

		template<TIsMatrix<T, 4, 4> IMatrix, TIsVector<T, 3> IPoint>
		static IPoint TransformPoint(const IMatrix& M, const IPoint& P);

		template<TIsVector<T, 3> IPoint>
		IPoint TransformPoint(const IPoint& P) const
		{
			return TransformPoint(*this, Vector3<T>(P));
		}

		template<TIsMatrix<T, 4, 4> IMatrix, TIsVector<T, 3> IVector>
		static IVector TransformVector(const IMatrix& M, const IVector& V)
		{
			return TransformVector(Matrix4x4<T>(M), Vector3<T>(V));
		}

		template<TIsVector<T, 3> IVector>
		IVector TransformVector(const IVector& V) const
		{
			return TransformVector(*this, Vector3<T>(V));
		}
	};

	template<>
	struct Matrix4x4<void>
	{
#define XM_C(v) reinterpret_cast<const DirectX::XMMATRIX&>(v)
#define XM_V(v) reinterpret_cast<DirectX::XMMATRIX&>(v)
#define MX_C(v) reinterpret_cast<const Matrix4x4<T>&>(v)
#define MX_V(v) reinterpret_cast<Matrix4x4<T>&>(v)

#define ASSIGN(x, y) x = reinterpret_cast<std::remove_reference_t<decltype(x)>&>(y)
#define FLOAT3(x) DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(&x))
#define FLOAT4(x) reinterpret_cast<const DirectX::XMVECTOR&>(x)

		template<class T>
		static constexpr T Determinant(const Matrix4x4<T>& M)
		{
			if constexpr (std::same_as<T, float> && std::is_constant_evaluated() == false)
			{
				using namespace DirectX;
				auto v = XMMatrixDeterminant(XM_C(M));
				return XMVectorGetX(v);
			}
			else
			{
				return
					M[0][0] * (M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
						M[1][2] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) +
						M[1][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1])) -
					M[0][1] * (M[1][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
						M[1][2] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) +
						M[1][3] * (M[2][0] * M[3][2] - M[2][2] * M[3][0])) +
					M[0][2] * (M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
						M[1][1] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) +
						M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0])) -
					M[0][3] * (M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
						M[1][1] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]) +
						M[1][2] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]));
			}
		}

		template<class T>
		static constexpr Matrix4x4<T> Inverse(const Matrix4x4<T>& M)
		{
			if constexpr (std::same_as<T, float> && std::is_constant_evaluated() == false)
			{
				using namespace DirectX;
				return MX_V(XMMatrixInverse(nullptr, XM_V(M)));
			}
			else
			{
				return Matrix<>::Inverse(M);
			}
		}

		template<class T>
		static constexpr bool Decompose(const Matrix4x4<T>& M, Translate3D<T>& translate, Scale3D<T>& scale, Quaternion<T>& rotation)
		{
			if constexpr (std::same_as<T, float> && std::is_constant_evaluated() == false)
			{
				using namespace DirectX;
				XMVECTOR xmtrans, xmscale, xmquat;
				bool success = XMMatrixDecompose(&xmtrans, &xmscale, &xmquat, XM_C(M));
				ASSIGN(translate, xmtrans);
				ASSIGN(scale, xmscale);
				ASSIGN(rotation, xmquat);
				return success;
			}
			else
			{
				// Extract Translation
				translate = Translate3D<T>(M[0][3], M[1][3], M[2][3]);

				// Extract Scale
				scale = Scale3D<T>(
					Math::Sqrt(M[0][0] * M[0][0] + M[0][1] * M[0][1] + M[0][2] * M[0][2]),
					Math::Sqrt(M[1][0] * M[1][0] + M[1][1] * M[1][1] + M[1][2] * M[1][2]),
					Math::Sqrt(M[2][0] * M[2][0] + M[2][1] * M[2][1] + M[2][2] * M[2][2])
				);

				// Normalize rotation matrix by removing scale
				T m00 = M[0][0] / scale.X, m01 = M[0][1] / scale.X, m02 = M[0][2] / scale.X;
				T m10 = M[1][0] / scale.Y, m11 = M[1][1] / scale.Y, m12 = M[1][2] / scale.Y;
				T m20 = M[2][0] / scale.Z, m21 = M[2][1] / scale.Z, m22 = M[2][2] / scale.Z;

				// Extract Rotation as Quaternion
				T qw = Math::Sqrt(1.0 + m00 + m11 + m22) / 2.0;
				T qx = (m21 - m12) / (4.0 * qw);
				T qy = (m02 - m20) / (4.0 * qw);
				T qz = (m10 - m01) / (4.0 * qw);

				rotation = Quaternion<T>(qw, qx, qy, qz);
			}
		}

		template<class T>
		static constexpr Vector3<T> TransformPoint(const Matrix4x4<T>& M, const Vector3<T>& P)
		{
			if constexpr (std::same_as<T, float> && std::is_constant_evaluated() == false)
			{
				using namespace DirectX;
				auto r = XMVector3Transform(FLOAT3(P), XM_C(M));
				return reinterpret_cast<Vector3<T>&>(r);
			}
			else
			{
				return Matrix<>::TransformPoint(M, P);
			}
		}

		template<class T>
		static constexpr Vector3<T> TransformVector(const Matrix4x4<T>& M, const Vector3<T>& V)
		{
			if constexpr (std::same_as<T, float> && std::is_constant_evaluated() == false)
			{
				using namespace DirectX;
				auto r = XMVector3TransformNormal(FLOAT3(V), XM_C(M));
				return reinterpret_cast<Vector3<T>&>(r);
			}
			else
			{
				return Matrix<>::TransformVector(M, V);
			}
		}

		template<class T>
		static constexpr Matrix4x4<T> Multiply(const Matrix4x4<T>& ML, const Matrix4x4<T>& MR)
		{
			if constexpr (std::same_as<T, float> && std::is_constant_evaluated() == false)
			{
				using namespace DirectX;
				return reinterpret_cast<Matrix4x4<T>&>(XMMatrixMultiply(XM_C(ML), XM_C(MR)));
			}
			else
			{
				return Matrix<>::Multiply(ML, MR);
			}
		}

		template<class T>
		static constexpr Matrix4x4<T> LookToLH(const Vector3<T>& EyePosition, const Vector3<T>& EyeDirection, const Vector3<T>& UpDirection)
		{
			if constexpr (std::same_as<T, float> && std::is_constant_evaluated() == false)
			{
				using namespace DirectX;
				auto location = FLOAT3(EyePosition);
				auto direction = FLOAT3(EyeDirection);
				auto up = FLOAT3(UpDirection);
				return reinterpret_cast<Matrix4x4<T>&>(XMMatrixLookToLH(location, direction, up));
			}
			else
			{
				auto R2 = Vector<>::Normalize(EyeDirection);
				auto R0 = Vector<>::Cross(UpDirection, R2);
				R0 = Vector<>::Normalize(R0);

				auto R1 = Vector<>::Cross(R2, R0);

				auto NegEyePosition = -EyePosition;

				auto D0 = Vector<>::Dot(R0, NegEyePosition);
				auto D1 = Vector<>::Dot(R1, NegEyePosition);
				auto D2 = Vector<>::Dot(R2, NegEyePosition);

				Matrix4x4<T> M;
				M[0] = Vector4<T>(R0, D0);
				M[1] = Vector4<T>(R1, D1);
				M[2] = Vector4<T>(R2, D2);
				M[3] = Vector4<T>(0, 0, 0, 1.0);

				return Matrix<>::Transpose(M);
			}
		}

		template<class T>
		static Matrix4x4<T> PerspectiveFovLH(const Radians<T>& fieldOfView, const T& aspectRatio, const T& near, const T& far)
		{
			if constexpr (std::same_as<T, float>)
			{
				using namespace DirectX;
				XMFLOAT4X4 m44;
				XMStoreFloat4x4(&m44, XMMatrixPerspectiveFovLH(fieldOfView.Value, aspectRatio, near, far));
				return reinterpret_cast<const Matrix4x4<T>&>(m44);
			}
			else
			{
				T SinFov;
				T CosFov;
				Math::SinCos(fieldOfView * 0.5, SinFov, CosFov);

				T Height = CosFov / SinFov;
				T Width = Height / aspectRatio;
				T fRange = far / (far - near);

				Matrix4x4<T> M;
				M[0][0] = Width;
				M[0][1] = 0.0;
				M[0][2] = 0.0;
				M[0][3] = 0.0;

				M[1][0] = 0.0;
				M[1][1] = Height;
				M[1][2] = 0.0;
				M[1][3] = 0.0;

				M[2][0] = 0.0;
				M[2][1] = 0.0;
				M[2][2] = fRange;
				M[2][3] = 1.0;

				M[3][0] = 0.0;
				M[3][1] = 0.0;
				M[3][2] = -fRange * near;
				M[3][3] = 0.0;
				return M;
			}
		}

		template<class T>
		static Matrix4x4<T> AffineTransformation(const Translate3D<T>& translate, const Scale3D<T>& scale, const Quaternion<T>& rotation)
		{
			if constexpr (std::same_as<T, float>)
			{
				using namespace DirectX;
				auto xmm = XMMatrixAffineTransformation(FLOAT3(scale), XMVectorZero(), FLOAT4(rotation), FLOAT3(translate));
				Matrix4x4<T> o;
				XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&o), xmm);
				return o;
			}
			else
			{
				// Create scaling matrix
				Matrix4x4<T> scaleMatrix = Matrix4x4<>::Scale(scale);

				// Create rotation matrix
				Matrix4x4<T> rotationMatrix = rotation.GetMatrix();

				// Create translation matrix
				Matrix4x4<T> translationMatrix = Matrix4x4<>::Translation(Vector3<T>(translate.X, translate.Y, translate.Z));

				// Combine transformations: Translation * Rotation * Scale
				return Matrix<>::Multiply(translationMatrix, rotationMatrix, scaleMatrix);
			}
		}

		template<class T>
		static Matrix4x4<T> Translation(const Vector3<T>& translate)
		{
			if constexpr (std::same_as<T, float>)
			{
				using namespace DirectX;
				return reinterpret_cast<Matrix4x4<T>&>(XMMatrixTranslation(translate.X, translate.Y, translate.Z));
			}
			else
			{
				// Start with an identity matrix
				Matrix4x4<T> M = Matrix4x4<T>::Identity();

				// Set the translation components
				M[0][3] = translate.X;
				M[1][3] = translate.Y;
				M[2][3] = translate.Z;

				return M;
			}
		}

		template<class T>
		static Matrix4x4<T> Scale(const Scale3D<T>& scale)
		{
			if constexpr (std::same_as<T, float>)
			{
				using namespace DirectX;
				return reinterpret_cast<Matrix4x4<T>&>(XMMatrixScaling(scale.X, scale.Y, scale.Z));
			}
			else
			{
				// Start with an identity matrix
				Matrix4x4<T> M = Matrix4x4<T>::Identity();

				// Set the translation components
				M[0][0] = scale.X;
				M[1][1] = scale.Y;
				M[2][2] = scale.Z;

				return M;
			}
		}

		template<class T>
		static Matrix4x4<T> Rotation(const Vector3<T>& Axis, Radians<T> Angle)
		{
			if constexpr (std::same_as<T, float>)
			{
				using namespace DirectX;
				return reinterpret_cast<Matrix4x4<T>&>(XMMatrixRotationAxis(FLOAT3(Axis), Angle.Value));
			}
			else
			{
				T x = Axis.X;
				T y = Axis.Y;
				T z = Axis.Z;
				T c, s;
				Math::SinCos(Angle, s, c);
				T t = 1 - c;

				Matrix4x4<T> M;
				M[0][0] = t * x * x + c;
				M[0][1] = t * x * y - s * z;
				M[0][2] = t * x * z + s * y;
				M[0][3] = 0;

				M[1][0] = t * x * y + s * z;
				M[1][1] = t * y * y + c;
				M[1][2] = t * y * z - s * x;
				M[1][3] = 0;

				M[2][0] = t * x * z - s * y;
				M[2][1] = t * y * z + s * x;
				M[2][2] = t * z * z + c;
				M[2][3] = 0;

				M[3][0] = 0;
				M[3][1] = 0;
				M[3][2] = 0;
				M[3][3] = 1;

				return M;
			}
		}

		template<class T>
		static Matrix4x4<T> RotationX(Radians<T> Angle)
		{
			if constexpr (std::same_as<T, float>)
			{
				using namespace DirectX;
				return reinterpret_cast<Matrix4x4<T>&>(XMMatrixRotationX(Angle.Value));
			}
			else
			{
				T c, s;
				Math::SinCos(Angle, s, c);

				Matrix4x4<T> M = Matrix4x4<T>::Identity();
				M[1][1] = c;
				M[1][2] = -s;
				M[2][1] = s;
				M[2][2] = c;

				return M;
			}
		}

		template<class T>
		static Matrix4x4<T> RotationY(Radians<T> Angle)
		{
			if constexpr (std::same_as<T, float>)
			{
				using namespace DirectX;
				return reinterpret_cast<Matrix4x4<T>&>(XMMatrixRotationY(Angle.Value));
			}
			else
			{
				T c, s;
				Math::SinCos(Angle, s, c);

				Matrix4x4<T> M = Matrix4x4<T>::Identity();
				M[0][0] = c;
				M[0][2] = s;
				M[2][0] = -s;
				M[2][2] = c;

				return M;
			}
		}

		template<class T>
		static Matrix4x4<T> RotationZ(Radians<T> Angle)
		{
			if constexpr (std::same_as<T, float>)
			{
				using namespace DirectX;
				return reinterpret_cast<Matrix4x4<T>&>(XMMatrixRotationZ(Angle.Value));
			}
			else
			{
				T c, s;
				Math::SinCos(Angle, s, c);

				Matrix4x4<T> M = Matrix4x4<T>::Identity();
				M[0][0] = c;
				M[0][1] = -s;
				M[1][0] = s;
				M[1][1] = c;

				return M;
			}
		}
#undef XM_C
#undef XM_V
#undef MX_C
#undef MX_V

#undef ASSIGN
#undef FLOAT3
#undef FLOAT4
	};

	using Matrix4x4F = Matrix4x4<float>;

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::Inverse() const
	{
		return Inverse(*this);
	}

	template<class T>
	template<TIsMatrix<T, 4, 4> IMatrix, TIsVector<T, 3> IPoint>
	IPoint Matrix4x4<T>::TransformPoint(const IMatrix& M, const IPoint& P)
	{
		return Matrix4x4<>::TransformPoint(Matrix4x4<T>(M), Vector3<T>(P));
	}

	template<class T>
	template<TIsVector<T, 3> ILocation, TIsVector<T, 3> IDir, TIsVector<T, 3> IUp>
	Matrix4x4<T> Matrix4x4<T>::LookToLH(const ILocation& Location, const IDir& Dir, const IUp& Up)
	{
		return LookToLH(Vector3<T>(Location), Vector3<T>(Dir), Vector3<T>(Up));
	}

	template<class T>
	template<TIsMatrix<T, 4, 3> IMatrix, TIsVector<T, 3> ITranslation, TIsVector<T, 3> IScale, TIsVector<T, 4> IQuaternion>
	auto Matrix4x4<T>::AffineTransformation(const ITranslation& translate, const IScale& scale, const IQuaternion& rot)
	{
		Matrix4x4 M = Matrix4x4<>::AffineTransformation(Translate3D<T>(translate), Scale3D<T>(scale), Quaternion<T>(rot));
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
}
