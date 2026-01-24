// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Actor.h"
#include "GameObject.gen.h"

namespace Ayla
{
	class Transform;

	ACLASS()
	class ENGINE_API GameObject : public Actor
	{
		GENERATED_BODY()

	private:
		SharedPtr<Transform> m_Transform;

	public:
		GameObject();
		virtual ~GameObject() noexcept override;

		SharedPtr<Transform> GetTransform() const noexcept;
	};
}