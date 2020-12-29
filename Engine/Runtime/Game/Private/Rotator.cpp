// Copyright 2020 Aumoa.lib. All right reserved.

#include "Rotator.h"

using namespace std;

Rotator::Rotator()
{

}

Rotator::Rotator(const TDegrees<double>& pitch, const TDegrees<double>& yaw, const TDegrees<double>& roll)
{
	Pitch = pitch;
	Yaw = yaw;
	Roll = roll;
}

Rotator::Rotator(const Quaternion& rotation)
{
	const double X = rotation.X;
	const double Y = rotation.Y;
	const double Z = rotation.Z;
	const double W = rotation.W;

	const double singularityTest = Z * X - W * Y;
	const double yawY = 2.0 * (W * Z + X * Y);
	const double yawX = (1.0 - 2.0 * (Math::Square(Y) + Math::Square(Z)));

	constexpr double SINGULARITY_THRESHOLD = 0.4999995;
	if (singularityTest < -SINGULARITY_THRESHOLD)
	{
		Pitch = -90.0;
		Yaw = Math::Atan2(yawY, yawX);
		Roll = (-Yaw - (2.0 * Math::Atan2(X, W))).Normalized;
	}
	else if (singularityTest > SINGULARITY_THRESHOLD)
	{
		Pitch = 90.f;
		Yaw = Math::Atan2(yawY, yawX);
		Roll = (Yaw - (2.0 * Math::Atan2(X, W))).Normalized;
	}
	else
	{
		Pitch = Math::Asin(2.0 * singularityTest);
		Yaw = Math::Atan2(yawY, yawX);
		Roll = Math::Atan2(-2.0 * (W * X + Y * Z), 1.0 - 2.0 * (Math::Square(X) + Math::Square(Y)));
	}
}

Rotator::Rotator(const Rotator& copy)
{
	Pitch = copy.Pitch;
	Yaw = copy.Yaw;
	Roll = copy.Roll;
}

Rotator::~Rotator()
{

}

bool Rotator::Equals(const Rotator& rh) const
{
	return Pitch == rh.Pitch
		&& Yaw == rh.Yaw
		&& Roll == rh.Roll;
}

bool Rotator::NearlyEquals(const Rotator& rh, TDegrees<double> epsilon) const
{
	return Math::Abs(Pitch - rh.Pitch) <= epsilon
		&& Math::Abs(Yaw - rh.Yaw) <= epsilon
		&& Math::Abs(Roll - rh.Roll) <= epsilon;
}

size_t Rotator::GetHashCode() const
{
	return Vector3(Pitch.Value, Yaw.Value, Roll.Value).GetHashCode();
}

TRefPtr<String> Rotator::ToString() const
{
	return String::Format("{{Yaw: {0}, Pitch: {1}, Roll: {2}}}", Yaw, Pitch, Roll);
}

Vector3 Rotator::RotateVector(const Vector3& v) const
{
	return Matrix.TransformVector(v);
}

Vector3 Rotator::UnrotateVector(const Vector3& v) const
{
	return Matrix.Transposed.TransformVector(v);
}

Rotator& Rotator::Add(const TDegrees<double>& yawDelta, const TDegrees<double>& pitchDelta, const TDegrees<double>& rollDelta)
{
	Pitch += pitchDelta;
	Yaw += yawDelta;
	Roll += rollDelta;
	return *this;
}

Rotator Rotator::Clamp() const
{
	return { fmod(Pitch.Value, 360.0), fmod(Yaw.Value, 360.0), fmod(Roll.Value, 360.0) };
}

Rotator Rotator::Inverse_get() const
{
	return Rotation.Inverse;
}

Quaternion Rotator::Rotation_get() const
{
	const TDegrees<double> PitchNoWinding = Pitch.Clamped;
	const TDegrees<double> YawNoWinding = Yaw.Clamped;
	const TDegrees<double> RollNoWinding = Roll.Clamped;

	auto[SP, CP] = Math::SinCos<double>(PitchNoWinding * 0.5);
	auto[SY, CY] = Math::SinCos<double>(YawNoWinding * 0.5);
	auto[SR, CR] = Math::SinCos<double>(RollNoWinding * 0.5);

	Quaternion rotationQuat;
	rotationQuat.X = CR * SP * SY - SR * CP * CY;
	rotationQuat.Y = -CR * SP * CY - SR * CP * SY;
	rotationQuat.Z = CR * CP * SY - SR * SP * CY;
	rotationQuat.W = CR * CP * CY + SR * SP * SY;

	return rotationQuat;
}

Matrix4x4 Rotator::Matrix_get() const
{
	const Vector3 Origin = Vector3::Zero;

	auto[SP, CP] = Math::SinCos<double>(Pitch);
	auto[SY, CY] = Math::SinCos<double>(Yaw);
	auto[SR, CR] = Math::SinCos<double>(Roll);

	Matrix4x4 M;
	M[0][0] = CP * CY;
	M[0][1] = CP * SY;
	M[0][2] = SP;
	M[0][3] = 0.f;

	M[1][0] = SR * SP * CY - CR * SY;
	M[1][1] = SR * SP * SY + CR * CY;
	M[1][2] = -SR * CP;
	M[1][3] = 0.f;

	M[2][0] = -(CR * SP * CY + SR * SY);
	M[2][1] = CY * SR - CR * SP * SY;
	M[2][2] = CR * CP;
	M[2][3] = 0.f;

	M[3][0] = Origin.X;
	M[3][1] = Origin.Y;
	M[3][2] = Origin.Z;
	M[3][3] = 1.f;

	return M;
}

Rotator Rotator::operator -() const
{
	return { -Yaw, -Pitch, -Roll };
}

Rotator Rotator::operator +(const Rotator& right) const
{
	return { Yaw + right.Yaw, Pitch + right.Pitch, Roll + right.Roll };
}

Rotator Rotator::operator -(const Rotator& right) const
{
	return { Yaw - right.Yaw, Pitch - right.Pitch, Roll - right.Roll };
}

Rotator& Rotator::operator +=(const Rotator& right)
{
	Yaw += right.Yaw;
	Pitch += right.Pitch;
	Roll += right.Roll;
	return *this;
}

Rotator& Rotator::operator -=(const Rotator& right)
{
	Yaw -= right.Yaw;
	Pitch -= right.Pitch;
	Roll -= right.Roll;
	return *this;
}

bool Rotator::operator ==(const Rotator& right) const
{
	return Yaw == right.Yaw && Pitch == right.Pitch && Roll == right.Roll;
}

bool Rotator::operator !=(const Rotator& right) const
{
	return Yaw != right.Yaw || Pitch != right.Pitch || Roll != right.Roll;
}