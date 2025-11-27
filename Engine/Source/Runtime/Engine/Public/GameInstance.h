// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "GameInstance.gen.h"

namespace Ayla
{
	class Scene;
	class Engine;

	ACLASS()
	class ENGINE_API GameInstance : public SerializableObject
	{
		GENERATED_BODY()

	private:
		Engine* m_Engine = nullptr;
		SharedPtr<Scene> m_EntryScene;

	public:
		GameInstance();
		virtual ~GameInstance() noexcept override;

		AFUNCTION()
		SharedPtr<Engine> GetEngine();
		AFUNCTION()
		virtual SharedPtr<Scene> GetEntryScene();

	protected:
		AFUNCTION()
		void InternalSetEngine(SharedPtr<Engine> engine);
	};
}