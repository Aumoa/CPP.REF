// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Component.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/TransformInterface/Quaternion.h"
#include "Transform.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API Transform : public Component
	{
		GENERATED_BODY()

	private:
		Vector3F m_LocalPosition = Vector3F::Zero();
		Vector3F m_LocalScale = Vector3F(1.0f, 1.0f, 1.0f);
		QuaternionF m_LocalRotation = QuaternionF::Identity();

	public:
		Transform();
		virtual ~Transform() noexcept override;
	};
}