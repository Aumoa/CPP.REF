// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct GAME_API Rotator
{
	TDegrees<float> Pitch;
	TDegrees<float> Yaw;
	TDegrees<float> Roll;

	Rotator();
	Rotator(const TDegrees<float>& yaw, const TDegrees<float>& pitch, const TDegrees<float>& roll);
	Rotator(const Quaternion& rotation);
	Rotator(const Rotator& copy);
	~Rotator();
		
	bool Equals(const Rotator& rh) const;
	bool NearlyEquals(const Rotator& rh, TDegrees<float> epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	Vector3 RotateVector(const Vector3& v) const;
	Vector3 UnrotateVector(const Vector3& v) const;

	Rotator& Add(const TDegrees<float>& yawDelta, const TDegrees<float>& pitchDelta, const TDegrees<float>& rollDelta);
	Rotator Clamp() const;

	vs_property_get(Rotator, Inverse);
	Rotator Inverse_get() const;
	vs_property_get(Quaternion, Rotation);
	Quaternion Rotation_get() const;
	vs_property_get(Matrix4x4, Matrix);
	Matrix4x4 Matrix_get() const;

	Rotator operator -() const;
	Rotator operator +(const Rotator& right) const;
	Rotator operator -(const Rotator& right) const;
	Rotator& operator +=(const Rotator& right);
	Rotator& operator -=(const Rotator& right);
	bool operator ==(const Rotator& right) const;
	bool operator !=(const Rotator& right) const;
};