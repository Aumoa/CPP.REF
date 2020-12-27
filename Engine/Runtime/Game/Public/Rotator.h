// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game
{
	struct GAME_API Rotator
	{
		Core::Mathematics::TDegrees<double> Pitch;
		Core::Mathematics::TDegrees<double> Yaw;
		Core::Mathematics::TDegrees<double> Roll;

		Rotator();
		Rotator(const Core::Mathematics::TDegrees<double>& yaw, const Core::Mathematics::TDegrees<double>& pitch, const Core::Mathematics::TDegrees<double>& roll);
		Rotator(const Core::Numerics::Quaternion& rotation);
		Rotator(const Rotator& copy);
		~Rotator();
		
		bool Equals(const Rotator& rh) const;
		bool NearlyEquals(const Rotator& rh, Core::Mathematics::TDegrees<double> epsilon) const;
		size_t GetHashCode() const;
		Core::TRefPtr<Core::String> ToString() const;

		Core::Numerics::Vector3 RotateVector(const Core::Numerics::Vector3& v) const;
		Core::Numerics::Vector3 UnrotateVector(const Core::Numerics::Vector3& v) const;

		Rotator& Add(const Core::Mathematics::TDegrees<double>& yawDelta, const Core::Mathematics::TDegrees<double>& pitchDelta, const Core::Mathematics::TDegrees<double>& rollDelta);
		Rotator Clamp() const;

		vs_property_get(Rotator, Inverse);
		Rotator Inverse_get() const;
		vs_property_get(Core::Numerics::Quaternion, Rotation);
		Core::Numerics::Quaternion Rotation_get() const;
		vs_property_get(Core::Numerics::Matrix4x4, Matrix);
		Core::Numerics::Matrix4x4 Matrix_get() const;

		Rotator operator -() const;
		Rotator operator +(const Rotator& right) const;
		Rotator operator -(const Rotator& right) const;
		Rotator& operator +=(const Rotator& right);
		Rotator& operator -=(const Rotator& right);
		bool operator ==(const Rotator& right) const;
		bool operator !=(const Rotator& right) const;
	};
}