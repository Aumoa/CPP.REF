// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Numerics/Quaternion.h"

#include <algorithm>
#include "IndexOutOfRangeException.h"
#include "HashHelper.h"
#include "DXMathMinimal.h"
#include "Numerics/AxisAlignedCube.h"
#include "Numerics/Matrix4x4.h"
#include "Numerics/Vector4.h"
#include "Mathematics/Math.h"
#include "Mathematics/TAngleDef.h"

using namespace std;

Quaternion Quaternion::Identity = Quaternion(0, 0, 0, 1.0f);

Quaternion::Quaternion()
{

}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

Quaternion::Quaternion(const Vector3& xyz, float w)
{
	X = xyz.X;
	Y = xyz.Y;
	Z = xyz.Z;
	W = w;
}

Quaternion::Quaternion(float splat)
{
	X = splat;
	Y = splat;
	Z = splat;
	W = splat;
}

Quaternion::Quaternion(const Quaternion& copy)
{
	X = copy.X;
	Y = copy.Y;
	Z = copy.Z;
	W = copy.W;
}

bool Quaternion::Equals(const Quaternion& rh) const
{
	return (*this) == rh;
}

bool Quaternion::NearlyEquals(const Quaternion& rh, float epsilon) const
{
	return abs(X - rh.X) <= epsilon
		&& abs(Y - rh.Y) <= epsilon
		&& abs(Z - rh.Z) <= epsilon
		&& abs(W - rh.W) <= epsilon;
}

size_t Quaternion::GetHashCode() const
{
	return HashHelper::GetHashCode(X)
		 ^ HashHelper::GetHashCode(Y)
		 ^ HashHelper::GetHashCode(Z)
		 ^ HashHelper::GetHashCode(W);
}

TRefPtr<String> Quaternion::ToString() const
{
	return String::Format(L"{{Axis: {0}, Angle: {1}}}", Axis, Angle);
}

float Quaternion::GetComponentOrDefault(size_t index) const
{
	if (!Contains(index))
	{
		return 0;
	}

	const float* ptr = &X;
	return ptr[index];
}

bool Quaternion::Contains(size_t index) const
{
	return index < Count;
}

size_t Quaternion::Count_get() const
{
	return 4;
}

pair<Vector3, float> Quaternion::ToAxisAngle() const
{
	return { Axis, Angle };
}

Vector3 Quaternion::RotateVector(const Vector3& v) const
{
	XMVECTOR xmv = XMLoadVector3(&v);

	XMVECTOR Q = XMVectorSet(X, Y, Z, 0);
	XMVECTOR T = 2.0f * XMVector3Cross(Q, xmv);
	XMVECTOR R = xmv + (W * T) + XMVector3Cross(Q, T);

	Vector3 r;
	XMStoreVector3(&r, R);
	return r;
}

Vector4 Quaternion::RotateVector(const Vector4& v) const
{
	return Vector4(RotateVector((const Vector3&)v), v.W);
}

float Quaternion::LengthSq_get() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

float Quaternion::Length_get() const
{
	return sqrt(LengthSq);
}

Quaternion Quaternion::Normalized_get() const
{
	return (*this) / Length;
}

Quaternion Quaternion::Direction_get() const
{
	return Normalized;
}

void Quaternion::Direction_set(Quaternion value)
{
	(*this) = value * Length;
}

Vector3 Quaternion::VectorPart_get() const
{
	return Vector3(X, Y, Z);
}

void Quaternion::VectorPart_set(Vector3 value)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
}

float Quaternion::Angle_get() const
{
	return 2.0f * acos(W);
}

Vector3 Quaternion::Axis_get() const
{
	float S = sqrt(max(1.0f - (W * W), 0.0f));

	if (S >= 0.0001f)
	{
		return Vector3(X / S, Y / S, Z / S);
	}

	return Vector3(1.0f, 0, 0);
}

Quaternion Quaternion::Inverse_get() const
{
	return Quaternion(-X, -Y, -Z, W);
}

Matrix4x4 Quaternion::Matrix_get() const
{
	XMMATRIX M = XMMatrixRotationQuaternion(XMLoadQuaternion(this));
	Matrix4x4 m;
	XMStoreMatrix4x4(&m, M);
	return m;
}

const float& Quaternion::operator [](size_t index) const
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	const float* ptr = &X;
	return ptr[index];
}

float& Quaternion::operator [](size_t index)
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	float* ptr = &X;
	return ptr[index];
}

Quaternion Quaternion::operator -() const
{
	return Quaternion(-X, -Y, -Z, -W);
}

Quaternion Quaternion::operator +(const Quaternion& value) const
{
	return Quaternion(X + value.X, Y + value.Y, Z + value.Z, W + value.W);
}

Quaternion Quaternion::operator -(const Quaternion& value) const
{
	return Quaternion(X - value.X, Y - value.Y, Z - value.Z, W - value.W);
}

Quaternion Quaternion::operator *(const Quaternion& value) const
{
	return Quaternion(X * value.X, Y * value.Y, Z * value.Z, W * value.W);
}

Quaternion Quaternion::operator /(const Quaternion& value) const
{
	return Quaternion(X / value.X, Y / value.Y, Z / value.Z, W / value.W);
}

float Quaternion::operator |(const Quaternion& value) const
{
	return DotProduct(*this, value);
}

Quaternion& Quaternion::operator +=(const Quaternion& right)
{
	X += right.X;
	Y += right.Y;
	Z += right.Z;
	W += right.W;
	return *this;
}

Quaternion& Quaternion::operator -=(const Quaternion& right)
{
	X -= right.X;
	Y -= right.Y;
	Z -= right.Z;
	W -= right.W;
	return *this;
}

Quaternion& Quaternion::operator *=(const Quaternion& right)
{
	X *= right.X;
	Y *= right.Y;
	Z *= right.Z;
	W *= right.W;
	return *this;
}

Quaternion& Quaternion::operator /=(const Quaternion& right)
{
	X /= right.X;
	Y /= right.Y;
	Z /= right.Z;
	W /= right.W;
	return *this;
}

float Quaternion::DotProduct(const Quaternion& left, const Quaternion& right)
{
	return left.X * right.X + left.Y * right.Y + left.Z * right.Z + left.W * right.W;
}

Quaternion Quaternion::FromAxisAngle(const Vector3& axis, TDegrees<float> angle)
{
	TRadians<float> rad = angle;
	XMVECTOR Axis = XMLoadVector3(&axis);
	XMVECTOR Q = XMQuaternionRotationNormal(Axis, rad.Value);
	Quaternion q;
	XMStoreQuaternion(&q, Q);
	return q;
}

Quaternion Quaternion::Concatenate(const Quaternion& left, const Quaternion& right)
{
	XMVECTOR Left = XMLoadQuaternion(&left);
	XMVECTOR Right = XMLoadQuaternion(&right);
	XMVECTOR Q = XMQuaternionMultiply(Left, Right);
	Quaternion q;
	XMStoreQuaternion(&q, Q);
	return q;
}

Quaternion Quaternion::FromMatrix(const Matrix4x4& rotationMatrix)
{
	XMMATRIX R = XMLoadMatrix4x4(&rotationMatrix);
	XMVECTOR Q = XMQuaternionRotationMatrix(R);
	Quaternion q;
	XMStoreQuaternion(&q, Q);
	return q;
}

Quaternion Quaternion::FromEuler(TDegrees<float> yaw, TDegrees<float> pitch, TDegrees<float> roll)
{
	auto yawq = FromAxisAngle(Vector3::Up, yaw.ToRadians().Value);
	auto pitchq = FromAxisAngle(Vector3::Right, pitch.ToRadians().Value);
	auto rollq = FromAxisAngle(Vector3::Forward, roll.ToRadians().Value);

	return Concatenate(Concatenate(yawq, pitchq), rollq);
}

Quaternion Quaternion::Lerp(const Quaternion& left, const Quaternion& right, float t)
{
	float Dot = DotProduct(left, right);
	float Bias = Dot >= 0 ? 1.0f : -1.0f;
	return (right * t) + (left * (Bias * (1 - t)));
}

Quaternion Quaternion::Slerp(const Quaternion& left, const Quaternion& right, float t)
{
	XMVECTOR Left = XMLoadQuaternion(&left);
	XMVECTOR Right = XMLoadQuaternion(&right);
	XMVECTOR Q = XMQuaternionSlerp(Left, Right, t);
	Quaternion q;
	XMStoreQuaternion(&q, Q);
	return q;
}

inline void ZUP(Vector3& lh)
{
	static const Matrix4x4 M = Matrix4x4(
		0, -1, 0, 0,
		0, 0, 1, 0,
		1, 0, 0, 0,
		0, 0, 0, 1
	);

	lh = M.TransformVector(lh);
}

Quaternion Quaternion::LookTo(const Vector3& forward, const Vector3& up)
{
	Quaternion t;

	Vector3 F = forward.Normalized;
	Vector3 U = up.Normalized;
	Vector3 R = Vector3::CrossProduct(up, forward).Normalized;
	U = Vector3::CrossProduct(F, R);

	XMMATRIX M = XMMatrixSet(
		R.X, R.Y, R.Z, 0,
		U.X, U.Y, U.Z, 0,
		F.X, F.Y, F.Z, 0,
		0, 0, 0, 1.0f
	);

	XMVECTOR Q = XMQuaternionRotationMatrix(M);
	XMStoreQuaternion(&t, Q);
	return t;
}

Quaternion operator +(float left, const Quaternion& right)
{
	return Quaternion(left) + right;
}

Quaternion operator -(float left, const Quaternion& right)
{
	return Quaternion(left) - right;
}

Quaternion operator *(float left, const Quaternion& right)
{
	return Quaternion(left) * right;
}

Quaternion operator /(float left, const Quaternion& right)
{
	return Quaternion(left) / right;
}