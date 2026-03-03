// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Components/Transform.h"
#include "LogEngine.h"

namespace Ayla
{
	Transform::Transform() : Super()
	{
	}

	Transform::~Transform() noexcept
	{
	}

	void Transform::GetPositionAndRotation(Vector3F* outPosition, QuaternionF* outRotation) const
	{
		LogEngine::Error(TEXT("Transform::GetPositionAndRotation is not implemented yet. Returning local position and rotation."));
		GetLocalPositionAndRotation(outPosition, outRotation);
	}
}