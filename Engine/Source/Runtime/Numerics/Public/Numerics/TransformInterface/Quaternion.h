// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<class>
	struct Matrix4x4;

	template<class T = void>
	struct Quaternion
	{
		Vector<T, 3> V;
		T W;

		constexpr Quaternion(T X, T Y, T Z, T W) : V(X, Y, Z), W(W)
		{
		}

		constexpr Quaternion(const Quaternion& Q) : V(Q.V), W(Q.W)
		{
		}

		template<TIsVector<T, 4> IQuaternion>
		constexpr Quaternion(const IQuaternion& Q) : V(Image(Q)), W(Real(Q))
		{
		}

		template<TIsVector<T, 3> IVector>
		constexpr Quaternion(const IVector& V, T W) : V(V), W(W)
		{
		}

		constexpr Quaternion& operator =(const Quaternion& Q)
		{
			V = Q.V;
			W = Q.W;
			return *this;
		}

	public:
		using Type = T;

		constexpr Quaternion(T S = 0) : V(S), W(S)
		{
		}

		static constexpr size_t Size()
		{
			return 4;
		}

		constexpr Quaternion operator -() const
		{
			return Quaternion(-V, -W);
		}

		constexpr const T& operator [](size_t N) const
		{
			switch (N)
			{
			case 0:
			case 1:
			case 2:
				return V[N];
			case 3:
			default:
				return W;
			}
		}

		constexpr T& operator [](size_t N)
		{
			switch (N)
			{
			case 0:
			case 1:
			case 2:
				return V[N];
			case 3:
			default:
				return W;
			}
		}

		template<TIsVector<T, 4> IQuaternion>
		constexpr Quaternion& operator =(const IQuaternion& Q)
		{
			V = Image(Q);
			W = Real(Q);
			return *this;
		}

	public:
		constexpr Quaternion Inverse() const;

		template<TIsVector<T, 4> IQuaternion>
		constexpr Quaternion Concatenate(const IQuaternion& Q) const;

		template<TIsVector<T, 3> IVector>
		constexpr IVector TransformPoint(const IVector& V) const;

		template<TIsVector<T, 4> IQuaternion, TIsVector<T, 3> IVector>
		static constexpr IVector TransformVector(const IQuaternion& QW, const IVector& V)
		{
			return TransformPoint(QW, V);
		}

		template<TIsVector<T, 3> IVector>
		constexpr auto TransformVector(const IVector& V) const
		{
			return TransformVector(*this, V);
		}

		static constexpr Quaternion Identity()
		{
			return Quaternion(0, 0, 0, 1.0);
		}

	public:
		NUMERICS_API static Quaternion FromAxisAngle(const Vector3<T>& Axis, Degrees<T> Angle);
		NUMERICS_API static Quaternion LookTo(const Vector3<T>& Forward, const Vector3<T>& Up);

		template<TIsVector<T, 3> IAxis>
		static Quaternion FromAxisAngle(const IAxis& Axis, Degrees<T> Angle)
		{
			return FromAxisAngle(Vector3<T>(Axis), Angle);
		}

		template<TIsVector<T, 3> IForward, TIsVector<T, 3> IUp>
		static Quaternion LookTo(const IForward& Forward, const IUp& Up)
		{
			return LookTo(Forward, Up);
		}

		template<TIsVector<T, 4> IQuaternion>
		static constexpr Vector3<T> Image(const IQuaternion& Q)
		{
			return Vector<>::Swizzling<0, 1, 2>(Q);
		}

		constexpr Vector3<T> Image() const
		{
			return Image(*this);
		}

		template<TIsVector<T, 4> IQuaternion>
		static constexpr T Real(const IQuaternion& Q)
		{
			return Q[3];
		}

		constexpr T Real() const
		{
			return Real(*this);
		}

		template<TIsVector<T, 4> IQuaternionL, TIsVector<T, 4> IQuaternionR>
		static constexpr Quaternion Multiply(const IQuaternionL& QL, const IQuaternionR& QR)
		{
			const Vector3<T> QLV = Image(QL);
			const T QLW = Real(QL);
			const Vector3<T> QRV = Image(QR);
			const T QRW = Real(QR);

			return Quaternion(QRV * QLW + QLV * QRW + Vector<>::Cross(QLV, QRV), QLW * QRW - Vector<>::Dot(QLV, QRV));
		}

		template<TIsVector<T, 4> IQuaternion>
		constexpr Quaternion Multiply(const IQuaternion& Q) const
		{
			return Multiply(*this, Q);
		}

		template<TIsVector<T, 4> IQuaternion>
		static constexpr Quaternion Conjugate(const IQuaternion& Q)
		{
			return Quaternion(-Image(Q), Real(Q));
		}

		constexpr Quaternion Conjugate() const
		{
			return Conjugate(*this);
		}

		template<TIsVector<T, 4> IQuaternionL, TIsVector<T, 4> IQuaternionR>
		static constexpr Quaternion Lerp(const IQuaternionL& QL, const IQuaternionR& QR, T t)
		{
			const T InvT = 1.0 - t;
			return Quaternion(Image(QL) * InvT + Image(QR) * t, Real(QL) * InvT + Real(QR) * t);
		}

		template<TIsVector<T, 4> IQuaternion>
		constexpr Quaternion Lerp(const IQuaternion& Q, T T) const
		{
			return Lerp(*this, Q, T);
		}

		template<TIsVector<T, 4> IQuaternionL, TIsVector<T, 4> IQuaternionR>
		static Quaternion Slerp(const IQuaternionL& QL, const IQuaternionR& QR, T t)
		{
			static constexpr const T Threshold = 0.9995;

			const auto V0 = Vector<>::Normalize(QL);
			const auto V1 = Vector<>::Normalize(QR);

			const T Dot = Vector<>::Dot(V0, V1);

			if (Dot < 0)
			{
				V1 = -V1;
				Dot = -Dot;
			}

			T Theta_0 = Math::Acos(Dot);
			T Theta = Theta_0 * t;
			if (Theta > Theta_0) return QR;

			if (Dot > Threshold)
			{
				return Vector<>::Normalize(Lerp(V0, V1, t));
			}

			T SinTheta = Math::Sin(Theta);
			T SinTheta0Inv = 1.0 / Math::Sin(Theta_0);

			T S0 = Math::Cos(Theta) - Dot * SinTheta * SinTheta0Inv;
			T S1 = SinTheta * SinTheta0Inv;

			return V0 * S0 + V1 * S1;
		}

		template<TIsVector<T, 4> IQuaternion>
		Quaternion Slerp(const IQuaternion& Q, T t) const
		{
			return Slerp(*this, Q, t);
		}

		template<TIsMatrix<T, 3, 3> IMatrix = Matrix4x4<T>, TIsVector<T, 4> IQuaternion>
		static constexpr IMatrix GetMatrix(const IQuaternion& Q)
		{
			IMatrix M = IMatrix::Identity();
			const T X = Q[0];
			const T Y = Q[1];
			const T Z = Q[2];
			const T W = Q[3];
			M[0][0] = 1.0 - 2.0 * (Y * Y + Z * Z);
			M[0][1] = 2.0 * (X * Y - Z * W);
			M[0][2] = 2.0 * (X * Z + Y * W);
			M[1][0] = 2.0 * (X * Y + Z * W);
			M[1][1] = 1.0 - 2.0 * (X * X + Z * Z);
			M[1][2] = 2.0 * (Y * Z - X * W);
			M[2][0] = 2.0 * (X * Z - Y * W);
			M[2][1] = 2.0 * (Y * Z + X * W);
			M[2][2] = 1.0 - 2.0 * (X * X + Y * Y);
			return M;
		}

		template<TIsMatrix<T, 3, 3> IMatrix = struct Matrix4x4>
		constexpr IMatrix GetMatrix() const
		{
			return GetMatrix<IMatrix>(*this);
		}

		template<TIsMatrix<T, 3, 3> IMatrix>
		static Quaternion FromMatrix(const IMatrix& M)
		{
			const T A1 = M[0][0];
			const T A2 = M[0][1];
			const T A3 = M[0][2];
			const T B1 = M[1][0];
			const T B2 = M[1][1];
			const T B3 = M[1][2];
			const T C1 = M[2][0];
			const T C2 = M[2][1];
			const T C3 = M[2][2];

			const T t = A1 + B2 + C3;
			Quaternion Q;

			if (t > 0.0)
			{
				const T S = Math::Sqrt(1 + t) * 2.0;
				Q[0] = (C2 - B3) / S;
				Q[1] = (A3 - C1) / S;
				Q[2] = (B1 - A2) / S;
				Q[3] = 0.25 * S;
			}
			else if (A1 > B2 && A1 > C3)
			{
				const T S = Math::Sqrt(1.0 + A1 - B2 - C3) * 2.0;
				Q[0] = 0.25 * S;
				Q[1] = (B1 + A2) / S;
				Q[2] = (A3 + C1) / S;
				Q[3] = (C2 - B3) / S;
			}
			else if (B2 > C3)
			{
				const T S = Math::Sqrt(1.0 + B2 - A1 - C3) * 2.0;
				Q[0] = (B1 + A2) / S;
				Q[1] = 0.25 * S;
				Q[2] = (C2 + B3) / S;
				Q[3] = (A3 - C1) / S;
			}
			else
			{
				const T S = Math::Sqrt(1.0 + C3 - A1 - B2) * 2.0;
				Q[0] = (A3 + C1) / S;
				Q[1] = (C2 + B3) / S;
				Q[2] = 0.25 * S;
				Q[3] = (B1 - A2) / S;
			}

			return Q;
		}

	public:
		String ToString() const
		{
			return Vector<>::ToString(*this);
		}

		constexpr bool NearlyEquals(const Quaternion& Q, T Epsilon) const
		{
			return Vector<>::NearlyEquals(*this, Q, Epsilon);
		}
	};

	struct NUMERICS_API SIMDQuaternion
	{
		SIMDQuaternion() = delete;

		static Quaternion<float> FromAxisAngle(const Vector3<float>& Axis, Degrees<float> Angle);
		static Quaternion<float> LookTo(const Vector3<float>& Forward, const Vector3<float>& Up);
	};

	template<>
	struct Quaternion<void>
	{
		template<class T, TIsVector<T, 4> IQuaternion>
		static constexpr Quaternion Inverse(const IQuaternion& Q)
		{
			return Quaternion<T>(-Image(Q), Real(Q));
		}

		template<class T, TIsVector<T, 4> IQuaternionL, TIsVector<T, 4> IQuaternionR>
		static constexpr Quaternion<T> Concatenate(const IQuaternionL& QL, const IQuaternionR& QR)
		{
			// Concatenate rotation is actually q2 * q1 instead of q1 * q2
			// So that's why QR goes q1 and QL goes q2.
			return Multiply(QR, QL);
		}

		template<class T, TIsVector<T, 4> IQuaternion, TIsVector<T, 3> IVector>
		static constexpr IVector TransformPoint(const IQuaternion& QW, const IVector& V)
		{
			const IVector Q(QW);
			const T W = QW.W;
			const IVector T = (Q ^ V) * 2.0;
			return V + (T * W) + (Q ^ T);
		}
	};

	template<class T>
	constexpr Quaternion<T> Quaternion<T>::Inverse() const
	{
		return Quaternion<>::Inverse(*this);
	}

	template<class T>
	template<TIsVector<T, 4> IQuaternion>
	constexpr Quaternion<T> Quaternion<T>::Concatenate(const IQuaternion& Q) const
	{
		return Concatenate(*this, Q);
	}

	template<class T>
	template<TIsVector<T, 3> IVector>
	constexpr IVector Quaternion<T>::TransformPoint(const IVector& V) const
	{
		return TransformPoint(*this, V);
	}

	using QuaternionF = Quaternion<float>;
	using QuaternionD = Quaternion<double>;
}