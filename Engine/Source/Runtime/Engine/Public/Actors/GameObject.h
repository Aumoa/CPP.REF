// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickTiming.h"
#include "Actors/Actor.h"
#include "GameObject.gen.h"

namespace Ayla
{
	class Scene;
	class Transform;

	ACLASS()
	class ENGINE_API GameObject : public Actor
	{
		GENERATED_BODY()

	private:
		Scene* m_Scene;
		SharedPtr<Transform> m_Transform;

	public:
		GameObject(Scene* scene);
		virtual ~GameObject() noexcept override;

		SharedPtr<Transform> GetTransform() const noexcept;
		Scene* GetScene() const noexcept { return m_Scene; }

		void Tick(TickTiming timing);
	};
}