// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Quaternion.h"

#include <algorithm>
#include "IndexOutOfRangeException.h"
#include "HashHelper.h"
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
	auto Q = Vector3(X, Y, Z);
	Vector3 T = 2.0 * Vector3::CrossProduct(Q, v);
	Vector3 result = v + (W * T) + Vector3::CrossProduct(Q, T);

	return result;
}

Vector4 Quaternion::RotateVector(const Vector4& v) const
{
	return Vector4(RotateVector(v.Cast<Vector3>()), v.W);
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

void Quaternion::Direction_set(const Quaternion& value)
{
	(*this) = value * Length;
}

Vector3 Quaternion::VectorPart_get() const
{
	return Vector3(X, Y, Z);
}

void Quaternion::VectorPart_set(const Vector3& value)
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
	float half = angle.ToRadians().Value * 0.5f;
	float vsin = sin(half);
	float vcos = cos(half);

	Quaternion quat;
	quat.X = axis.X * vsin;
	quat.Y = axis.Y * vsin;
	quat.Z = axis.Z * vsin;
	quat.W = vcos;

	return quat;
}

Quaternion Quaternion::Concatenate(const Quaternion& left, const Quaternion& right)
{
	Quaternion quaternion;
	float x = (right.Y * left.Z) - (right.Z * left.Y);
	float y = (right.Z * left.X) - (right.X * left.Z);
	float z = (right.X * left.Y) - (right.Y * left.X);
	float w = ((right.X * left.X) + (right.Y * left.Y)) + (right.Z * left.Z);
	quaternion.X = ((right.X * left.W) + (left.X * right.W)) + x;
	quaternion.Y = ((right.Y * left.W) + (left.Y * right.W)) + y;
	quaternion.Z = ((right.Z * left.W) + (left.Z * right.W)) + z;
	quaternion.W = (right.W * left.W) - w;
	return quaternion;
}

Quaternion Quaternion::FromMatrix(const Matrix4x4& rotationMatrix)
{
	float side = (rotationMatrix._11 + rotationMatrix._22) + rotationMatrix._33;

	Quaternion quaternion;
	if (side > 0)
	{
		float sq = sqrt(side + 1);
		float sqiv = 0.5f / sq;
		quaternion.W = sq * 0.5f;
		quaternion.X = (rotationMatrix._23 - rotationMatrix._32) * sqiv;
		quaternion.Y = (rotationMatrix._31 - rotationMatrix._13) * sqiv;
		quaternion.Z = (rotationMatrix._12 - rotationMatrix._21) * sqiv;
	}

	else if ((rotationMatrix._11 >= rotationMatrix._22) && (rotationMatrix._11 >= rotationMatrix._33))
	{
		float sq = sqrt(((1 + rotationMatrix._11) - rotationMatrix._22) - rotationMatrix._33);
		float sqiv = 0.5f / sq;
		quaternion.X = 0.5f * sq;
		quaternion.Y = (rotationMatrix._12 + rotationMatrix._21) * sqiv;
		quaternion.Z = (rotationMatrix._13 + rotationMatrix._31) * sqiv;
		quaternion.W = (rotationMatrix._23 - rotationMatrix._32) * sqiv;
	}

	else if (rotationMatrix._22 > rotationMatrix._33)
	{
		float sq = sqrt(((1 + rotationMatrix._22) - rotationMatrix._11) - rotationMatrix._33);
		float sqiv = 0.5f / sq;
		quaternion.X = (rotationMatrix._21 + rotationMatrix._12) * sqiv;
		quaternion.Y = 0.5f * sq;
		quaternion.Z = (rotationMatrix._32 + rotationMatrix._23) * sqiv;
		quaternion.W = (rotationMatrix._31 - rotationMatrix._13) * sqiv;
	}

	else
	{
		float sq = sqrt(((1 + rotationMatrix._33) - rotationMatrix._11) - rotationMatrix._22);
		float sqiv = 0.5f / sq;
		quaternion.X = (rotationMatrix._31 + rotationMatrix._13) * sqiv;
		quaternion.Y = (rotationMatrix._32 + rotationMatrix._23) * sqiv;
		quaternion.Z = 0.5f * sq;
		quaternion.W = (rotationMatrix._12 - rotationMatrix._21) * sqiv;
	}

	return quaternion;
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
	float Threshold = 0.9995f;

	Quaternion v0 = left.Normalized;
	Quaternion v1 = right.Normalized;

	float dot = DotProduct(v0, v1);

	if (dot < 0)
	{
		v1 = -v1;
		dot = -dot;
	}

	float theta_0 = acos(dot);
	float theta = theta_0 * t;
	theta = clamp(theta, 0.0f, 3.1415926535f);
	if (theta > theta_0)
	{
		return left;
	}

	if (dot > Threshold)
	{
		return Lerp(v0, v1, t).Normalized;
	}

	float sin_theta = sin(theta);
	float sin_theta_0 = sin(theta_0);

	float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
	float s1 = sin_theta / sin_theta_0;

	return v0 * s0 + v1 * s1;
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