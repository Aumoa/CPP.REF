// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "ActorComponent.h"

#include "Transform.h"

namespace SC::Runtime::Game::Components
{
	class GAME_API SceneComponent : public ActorComponent
	{
	public:
		using Super = ActorComponent;
		using This = SceneComponent;

	private:
		Transform transform;

	public:
		SceneComponent();
		~SceneComponent();

		vs_property(Transform, RelativeTransform);
		Transform RelativeTransform_get() const;
		void RelativeTransform_set(const Transform& value);
	};
}