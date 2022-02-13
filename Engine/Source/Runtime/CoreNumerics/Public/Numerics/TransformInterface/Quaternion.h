// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Numerics/VectorInterface/Vector.h"
#include "Mathematics/MathEx.h"
#include "Mathematics/Degrees.h"

struct Quaternion
{
	Vector<float, 3> V;
	float W;

	constexpr Quaternion(float X, float Y, float Z, float W) : V(X, Y, Z), W(W)
	{
	}

	constexpr Quaternion(const Quaternion& Q) : V(Q.V), W(Q.W)
	{
	}

	template<TIsVector<float, 4> IQuaternion>
	constexpr Quaternion(const IQuaternion& Q) : V(Image(Q)), W(Real(Q))
	{
	}

	template<TIsVector<float, 3> IVector>
	constexpr Quaternion(const IVector& V, float W) : V(V), W(W)
	{
	}

	constexpr Quaternion& operator =(const Quaternion& Q)
	{
		V = Q.V;
		W = Q.W;
		return *this;
	}

public:
	using Type = float;

	constexpr Quaternion(float S = 0) : V(S), W(S)
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

	constexpr const float& operator [](size_t N) const
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

	constexpr float& operator [](size_t N)
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
	
	template<TIsVector<float, 4> IQuaternion>
	constexpr Quaternion& operator =(const IQuaternion& Q)
	{
		V = Image(Q);
		W = Real(Q);
		return *this;
	}

public:
	template<TIsVector<float, 4> IQuaternion>
	static constexpr Quaternion Inverse(const IQuaternion& Q)
	{
		return Quaternion(-Image(Q), Real(Q));
	}

	constexpr Quaternion Inverse() const
	{
		return Inverse(*this);
	}

	template<TIsVector<float, 4> IQuaternionL, TIsVector<float, 4> IQuaternionR>
	static constexpr Quaternion Concatenate(const IQuaternionL& QL, const IQuaternionR& QR)
	{
		// Concatenate rotation is actually q2 * q1 instead of q1 * q2/
		// So that's why QR goes q1 and QL goes q2.
		return Multiply(QR, QL);
	}

	template<TIsVector<float, 4> IQuaternion>
	constexpr Quaternion Concatenate(const IQuaternion& Q) const
	{
		return Concatenate(*this, Q);
	}

	template<TIsVector<float, 4> IQuaternion, TIsVector<float, 3> IVector>
	static constexpr IVector TransformPoint(const IQuaternion& QW, const IVector& V)
	{
		const IVector Q(QW);
		const float W = QW.W;
		const IVector T = (Q ^ V) * 2.0f;
		return V + (T * W) + (Q ^ T);
	}

	template<TIsVector<float, 3> IVector>
	constexpr auto TransformPoint(const IVector& V) const
	{
		return TransformPoint(*this, V);
	}

	template<TIsVector<float, 4> IQuaternion, TIsVector<float, 3> IVector>
	static constexpr IVector TransformVector(const IQuaternion& QW, const IVector& V)
	{
		return TransformPoint(QW, V);
	}

	template<TIsVector<float, 3> IVector>
	constexpr auto TransformVector(const IVector& V) const
	{
		return TransformVector(*this, V);
	}

	static constexpr Quaternion Identity()
	{
		return Quaternion(0, 0, 0, 1.0f);
	}

public:
	CORENUMERICS_API static Quaternion FromAxisAngle(const Vector3& Axis, Degrees Angle);
	CORENUMERICS_API static Quaternion LookTo(const Vector3& Forward, const Vector3& Up);

	template<TIsVector<float, 3> IAxis>
	static Quaternion FromAxisAngle(const IAxis& Axis, Degrees Angle)
	{
		return FromAxisAngle(Vector3(Axis), Angle);
	}

	template<TIsVector<float, 3> IForward, TIsVector<float, 3> IUp>
	static Quaternion LookTo(const IForward& Forward, const IUp& Up)
	{
		return LookTo(Forward, Up);
	}

	template<TIsVector<float, 4> IQuaternion>
	static constexpr Vector3 Image(const IQuaternion& Q)
	{
		return Vector<>::Swizzling<0, 1, 2>(Q);
	}

	constexpr Vector3 Image() const
	{
		return Image(*this);
	}

	template<TIsVector<float, 4> IQuaternion>
	static constexpr float Real(const IQuaternion& Q)
	{
		return Q[3];
	}

	constexpr float Real() const
	{
		return Real(*this);
	}

	template<TIsVector<float, 4> IQuaternionL, TIsVector<float, 4> IQuaternionR>
	static constexpr Quaternion Multiply(const IQuaternionL& QL, const IQuaternionR& QR)
	{
		const Vector3 QLV = Image(QL);
		const float QLW = Real(QL);
		const Vector3 QRV = Image(QR);
		const float QRW = Real(QR);

		return Quaternion(QRV * QLW + QLV * QRW + Vector<>::Cross(QLV, QRV), QLW * QRW - Vector<>::Dot(QLV, QRV));
	}

	template<TIsVector<float, 4> IQuaternion>
	constexpr Quaternion Multiply(const IQuaternion& Q) const
	{
		return Multiply(*this, Q);
	}

	template<TIsVector<float, 4> IQuaternion>
	static constexpr Quaternion Conjugate(const IQuaternion& Q)
	{
		return Quaternion(-Image(Q), Real(Q));
	}

	constexpr Quaternion Conjugate() const
	{
		return Conjugate(*this);
	}

	template<TIsVector<float, 4> IQuaternionL, TIsVector<float, 4> IQuaternionR>
	static constexpr Quaternion Lerp(const IQuaternionL& QL, const IQuaternionR& QR, float T)
	{
		const float InvT = 1.0f - T;
		return Quaternion(Image(QL) * InvT + Image(QR) * T, Real(QL) * InvT + Real(QR) * T);
	}

	template<TIsVector<float, 4> IQuaternion>
	constexpr Quaternion Lerp(const IQuaternion& Q, float T) const
	{
		return Lerp(*this, Q, T);
	}

	template<TIsVector<float, 4> IQuaternionL, TIsVector<float, 4> IQuaternionR>
	static Quaternion Slerp(const IQuaternionL& QL, const IQuaternionR& QR, float T)
	{
		static constexpr const float Threshold = 0.9995f;

		const auto V0 = Vector<>::Normalize(QL);
		const auto V1 = Vector<>::Normalize(QR);

		const float Dot = Vector<>::Dot(V0, V1);

		if (Dot < 0)
		{
			V1 = -V1;
			Dot = -Dot;
		}

		float Theta_0 = MathEx::Acos(Dot);
		float Theta = Theta_0 * T;
		if (Theta > Theta_0) return QR;

		if (Dot > Threshold)
		{
			return Vector<>::Normalize(Lerp(V0, V1, T));
		}

		float SinTheta = MathEx::Sin(Theta);
		float SinTheta0Inv = 1.0f / MathEx::Sin(Theta_0);

		float S0 = MathEx::Cos(Theta) - Dot * SinTheta * SinTheta0Inv;
		float S1 = SinTheta * SinTheta0Inv;

		return V0 * S0 + V1 * S1;
	}

	template<TIsVector<float, 4> IQuaternion>
	Quaternion Slerp(const IQuaternion& Q, float T) const
	{
		return Slerp(*this, Q, T);
	}

	template<TIsMatrix<float, 3, 3> IMatrix = struct Matrix4x4, TIsVector<float, 4> IQuaternion>
	static constexpr IMatrix GetMatrix(const IQuaternion& Q)
	{
		IMatrix M = IMatrix::Identity();
		const float X = Q[0];
		const float Y = Q[1];
		const float Z = Q[2];
		const float W = Q[3];
		M[0][0] = 1.0f - 2.0F * (Y * Y + Z * Z);
		M[0][1] = 2.0F * (X * Y - Z * W);
		M[0][2] = 2.0F * (X * Z + Y * W);
		M[1][0] = 2.0F * (X * Y + Z * W);
		M[1][1] = 1.0F - 2.0F * (X * X + Z * Z);
		M[1][2] = 2.0F * (Y * Z - X * W);
		M[2][0] = 2.0F * (X * Z - Y * W);
		M[2][1] = 2.0F * (Y * Z + X * W);
		M[2][2] = 1.0F - 2.0F * (X * X + Y * Y);
		return M;
	}

	template<TIsMatrix<float, 3, 3> IMatrix = struct Matrix4x4>
	constexpr IMatrix GetMatrix() const
	{
		return GetMatrix<IMatrix>(*this);
	}

	template<TIsMatrix<float, 3, 3> IMatrix>
	static Quaternion FromMatrix(const IMatrix& M)
	{
		const float A1 = M[0][0];
		const float A2 = M[0][1];
		const float A3 = M[0][2];
		const float B1 = M[1][0];
		const float B2 = M[1][1];
		const float B3 = M[1][2];
		const float C1 = M[2][0];
		const float C2 = M[2][1];
		const float C3 = M[2][2];

		const float T = A1 + B2 + C3;
		Quaternion Q;

		if (T > 0f)
		{
			const float S = MathEx::Sqrt(1 + T) * 2.0f;
			Q[0] = (C2 - B3) / S;
			Q[1] = (A3 - C1) / S;
			Q[2] = (B1 - A2) / S;
			Q[3] = 0.25f * S;
		}
		else if (A1 > B2 && A1 > C3)
		{
			const float S = MathEx::Sqrt(1.0f + A1 - B2 - C3) * 2.0f;
			Q[0] = 0.25f * S;
			Q[1] = (B1 + A2) / S;
			Q[2] = (A3 + C1) / S;
			Q[3] = (C2 - B3) / S;
		}
		else if (B2 > C3)
		{
			const float S = MathEx::Sqrt(1.0f + B2 - A1 - C3) * 2.0f;
			Q[0] = (B1 + A2) / S;
			Q[1] = 0.25f * S;
			Q[2] = (C2 + B3) / S;
			Q[3] = (A3 - C1) / S;
		}
		else
		{
			const float S = MathEx::Sqrt(1.0f + C3 - A1 - B2) * 2.0f;
			Q[0] = (A3 + C1) / S;
			Q[1] = (C2 + B3) / S;
			Q[2] = 0.25f * S;
			Q[3] = (B1 - A2) / S;
		}
		
		return Q;
	}

public:
	std::wstring ToString(std::wstring_view FormatArgs) const
	{
		return Vector<>::ToString(*this, FormatArgs);
	}

	constexpr bool NearlyEquals(const Quaternion& Q, float Epsilon) const
	{
		return Vector<>::NearlyEquals(*this, Q, Epsilon);
	}
};