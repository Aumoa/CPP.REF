// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Rotator.h"

struct GAME_API Transform
{
	Vector3 Translation;
	Vector3 Scale;
	Quaternion Rotation;

	Transform();
	Transform(const Vector3& translation, const Vector3& scale = Vector3::One, const Quaternion& rotation = Quaternion::Identity);
	Transform(const Transform& copy);
	~Transform();

	bool Equals(const Transform& rh) const;
	bool NearlyEquals(const Transform& rh, double epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	Vector3 TransformVector(const Vector3& v) const;
	Vector4 TransformVector(const Vector4& v) const;

	bool operator ==(const Transform& rh) const;
	bool operator !=(const Transform& rh) const;

	static Transform Identity;
};