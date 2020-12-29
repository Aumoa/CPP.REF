// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Rotator.h"

namespace SC::Runtime::Game
{
	struct GAME_API Transform
	{
		Core::Numerics::Vector3 Translation;
		Core::Numerics::Vector3 Scale;
		Core::Numerics::Quaternion Rotation;

		Transform();
		Transform(const Core::Numerics::Vector3& translation, const Core::Numerics::Vector3& scale = Core::Numerics::Vector3::One, const Core::Numerics::Quaternion& rotation = Core::Numerics::Quaternion::Identity);
		Transform(const Transform& copy);
		~Transform();

		bool Equals(const Transform& rh) const;
		bool NearlyEquals(const Transform& rh, double epsilon) const;
		size_t GetHashCode() const;
		Core::TRefPtr<Core::String> ToString() const;

		Core::Numerics::Vector3 TransformVector(const Core::Numerics::Vector3& v) const;
		Core::Numerics::Vector4 TransformVector(const Core::Numerics::Vector4& v) const;

		bool operator ==(const Transform& rh) const;
		bool operator !=(const Transform& rh) const;

		static Transform Identity;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::Transform;
#endif