// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Quaternion.h"

#include <algorithm>
#include "IndexOutOfRangeException.h"
#include "HashHelper.h"
#include "Numerics/AxisAlignedCube.h"
#include "Numerics/Matrix4x4.h"
#include "Numerics/Vector4.h"
#include "Mathematics/TDegrees.h"
#include "Mathematics/TRadians.h"
#include "Mathematics/Math.h"
#include "Mathematics/Math.inl"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Core::Numerics;
using namespace SC::Runtime::Core::Mathematics;
using namespace std;

Quaternion Quaternion::Identity = Quaternion(0, 0, 0, 1);

Quaternion::Quaternion()
{

}

Quaternion::Quaternion(double x, double y, double z, double w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

Quaternion::Quaternion(const Vector3& xyz, double w)
{
	X = xyz.X;
	Y = xyz.Y;
	Z = xyz.Z;
	W = w;
}

Quaternion::Quaternion(double splat)
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

bool Quaternion::NearlyEquals(const Quaternion& rh, double epsilon) const
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

double Quaternion::GetComponentOrDefault(size_t index) const
{
	if (!Contains(index))
	{
		return 0;
	}

	const double* ptr = &X;
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

pair<Vector3, double> Quaternion::ToAxisAngle() const
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

double Quaternion::LengthSq_get() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

double Quaternion::Length_get() const
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

double Quaternion::Angle_get() const
{
	return 2.0 * acos(W);
}

Vector3 Quaternion::Axis_get() const
{
	double S = sqrt(max(1.0 - (W * W), 0.0));

	if (S >= 0.0001)
	{
		return Vector3(X / S, Y / S, Z / S);
	}

	return Vector3(1, 0, 0);
}

Quaternion Quaternion::Inverse_get() const
{
	return Quaternion(-X, -Y, -Z, W);
}

const double& Quaternion::operator [](size_t index) const
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	const double* ptr = &X;
	return ptr[index];
}

double& Quaternion::operator [](size_t index)
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	double* ptr = &X;
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

double Quaternion::operator |(const Quaternion& value) const
{
	return DotProduct(*this, value);
}

bool Quaternion::operator ==(const Quaternion& value) const
{
	return X == value.X && Y == value.Y && Z == value.Z && W == value.W;
}

bool Quaternion::operator !=(const Quaternion& value) const
{
	return X != value.X || Y != value.Y || Z != value.Z || W != value.W;
}

bool Quaternion::operator < (const Quaternion& right) const
{
	if (X < right.X)
	{
		return true;
	}
	else if (X > right.X)
	{
		return false;
	}
	else if (Y < right.Y)
	{
		return true;
	}
	else if (Y > right.Y)
	{
		return false;
	}
	else if (Z < right.Z)
	{
		return true;
	}
	else if (Z > right.Z)
	{
		return false;
	}
	else if (W < right.W)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Quaternion::operator <=(const Quaternion& right) const
{
	if (X < right.X)
	{
		return true;
	}
	else if (X > right.X)
	{
		return false;
	}
	else if (Y < right.Y)
	{
		return true;
	}
	else if (Y > right.Y)
	{
		return false;
	}
	else if (Z < right.Z)
	{
		return true;
	}
	else if (Z > right.Z)
	{
		return false;
	}
	else if (W <= right.W)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Quaternion::operator > (const Quaternion& right) const
{
	if (X > right.X)
	{
		return true;
	}
	else if (X < right.X)
	{
		return false;
	}
	else if (Y > right.Y)
	{
		return true;
	}
	else if (Y < right.Y)
	{
		return false;
	}
	else if (Z > right.Z)
	{
		return true;
	}
	else if (Z < right.Z)
	{
		return false;
	}
	else if (W > right.W)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Quaternion::operator >=(const Quaternion& right) const
{
	if (X > right.X)
	{
		return true;
	}
	else if (X < right.X)
	{
		return false;
	}
	else if (Y > right.Y)
	{
		return true;
	}
	else if (Y < right.Y)
	{
		return false;
	}
	else if (Z > right.Z)
	{
		return true;
	}
	else if (Z < right.Z)
	{
		return false;
	}
	else if (W >= right.W)
	{
		return true;
	}
	else
	{
		return false;
	}
}

weak_ordering Quaternion::operator <=>(const Quaternion& right) const
{
	if (X < right.X)
	{
		return weak_ordering::less;
	}
	else if (X > right.X)
	{
		return weak_ordering::greater;
	}
	else if (Y < right.Y)
	{
		return weak_ordering::less;
	}
	else if (Y > right.Y)
	{
		return weak_ordering::greater;
	}
	else if (Z < right.Z)
	{
		return weak_ordering::less;
	}
	else if (Z > right.Z)
	{
		return weak_ordering::greater;
	}
	else if (W < right.W)
	{
		return weak_ordering::less;
	}
	else if (W > right.W)
	{
		return weak_ordering::greater;
	}
	else
	{
		return weak_ordering::equivalent;
	}
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

double Quaternion::DotProduct(const Quaternion& left, const Quaternion& right)
{
	return left.X * right.X + left.Y * right.Y + left.Z * right.Z + left.W * right.W;
}

Quaternion Quaternion::FromAxisAngle(const Vector3& axis, TDegrees<double> angle)
{
	double half = angle.ToRadians().Value * 0.5;
	double vsin = sin(half);
	double vcos = cos(half);

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
	double x = (right.Y * left.Z) - (right.Z * left.Y);
	double y = (right.Z * left.X) - (right.X * left.Z);
	double z = (right.X * left.Y) - (right.Y * left.X);
	double w = ((right.X * left.X) + (right.Y * left.Y)) + (right.Z * left.Z);
	quaternion.X = ((right.X * left.W) + (left.X * right.W)) + x;
	quaternion.Y = ((right.Y * left.W) + (left.Y * right.W)) + y;
	quaternion.Z = ((right.Z * left.W) + (left.Z * right.W)) + z;
	quaternion.W = (right.W * left.W) - w;
	return quaternion;
}

Quaternion Quaternion::FromMatrix(const Matrix4x4& rotationMatrix)
{
	double side = (rotationMatrix._11 + rotationMatrix._22) + rotationMatrix._33;

	Quaternion quaternion;
	if (side > 0)
	{
		double sq = sqrt(side + 1);
		double sqiv = 0.5 / sq;
		quaternion.W = sq * 0.5;
		quaternion.X = (rotationMatrix._23 - rotationMatrix._32) * sqiv;
		quaternion.Y = (rotationMatrix._31 - rotationMatrix._13) * sqiv;
		quaternion.Z = (rotationMatrix._12 - rotationMatrix._21) * sqiv;
	}

	else if ((rotationMatrix._11 >= rotationMatrix._22) && (rotationMatrix._11 >= rotationMatrix._33))
	{
		double sq = sqrt(((1 + rotationMatrix._11) - rotationMatrix._22) - rotationMatrix._33);
		double sqiv = 0.5 / sq;
		quaternion.X = 0.5 * sq;
		quaternion.Y = (rotationMatrix._12 + rotationMatrix._21) * sqiv;
		quaternion.Z = (rotationMatrix._13 + rotationMatrix._31) * sqiv;
		quaternion.W = (rotationMatrix._23 - rotationMatrix._32) * sqiv;
	}

	else if (rotationMatrix._22 > rotationMatrix._33)
	{
		double sq = sqrt(((1 + rotationMatrix._22) - rotationMatrix._11) - rotationMatrix._33);
		double sqiv = 0.5 / sq;
		quaternion.X = (rotationMatrix._21 + rotationMatrix._12) * sqiv;
		quaternion.Y = 0.5 * sq;
		quaternion.Z = (rotationMatrix._32 + rotationMatrix._23) * sqiv;
		quaternion.W = (rotationMatrix._31 - rotationMatrix._13) * sqiv;
	}

	else
	{
		double sq = sqrt(((1 + rotationMatrix._33) - rotationMatrix._11) - rotationMatrix._22);
		double sqiv = 0.5 / sq;
		quaternion.X = (rotationMatrix._31 + rotationMatrix._13) * sqiv;
		quaternion.Y = (rotationMatrix._32 + rotationMatrix._23) * sqiv;
		quaternion.Z = 0.5 * sq;
		quaternion.W = (rotationMatrix._12 - rotationMatrix._21) * sqiv;
	}

	return quaternion;
}

Quaternion Quaternion::FromEuler(TDegrees<double> yaw, TDegrees<double> pitch, TDegrees<double> roll)
{
	auto yawq = FromAxisAngle(Vector3::Up, yaw.ToRadians().Value);
	auto pitchq = FromAxisAngle(Vector3::Right, pitch.ToRadians().Value);
	auto rollq = FromAxisAngle(Vector3::Forward, roll.ToRadians().Value);

	return Concatenate(Concatenate(yawq, pitchq), rollq);
}

Quaternion Quaternion::Lerp(const Quaternion& left, const Quaternion& right, double t)
{
	double Dot = DotProduct(left, right);
	double Bias = Dot >= 0 ? 1.0 : -1.0;
	return (right * t) + (left * (Bias * (1 - t)));
}

Quaternion Quaternion::Slerp(const Quaternion& left, const Quaternion& right, double t)
{
	double Threshold = 0.9995;

	Quaternion v0 = left.Normalized;
	Quaternion v1 = right.Normalized;

	double dot = DotProduct(v0, v1);

	if (dot < 0)
	{
		v1 = -v1;
		dot = -dot;
	}

	double theta_0 = acos(dot);
	double theta = theta_0 * t;
	theta = clamp(theta, 0.0, 3.1415926535);
	if (theta > theta_0)
	{
		return left;
	}

	if (dot > Threshold)
	{
		return Lerp(v0, v1, t).Normalized;
	}

	double sin_theta = sin(theta);
	double sin_theta_0 = sin(theta_0);

	double s0 = cos(theta) - dot * sin_theta / sin_theta_0;
	double s1 = sin_theta / sin_theta_0;

	return v0 * s0 + v1 * s1;
}

Quaternion Numerics::operator +(double left, const Quaternion& right)
{
	return Quaternion(left) + right;
}

Quaternion Numerics::operator -(double left, const Quaternion& right)
{
	return Quaternion(left) - right;
}

Quaternion Numerics::operator *(double left, const Quaternion& right)
{
	return Quaternion(left) * right;
}

Quaternion Numerics::operator /(double left, const Quaternion& right)
{
	return Quaternion(left) / right;
}