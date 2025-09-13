// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "GameInstance.gen.h"

namespace Ayla
{
	class Scene;

	ACLASS()
	class ENGINE_API GameInstance : public SerializableObject
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		std::shared_ptr<Scene> m_EntryScene;

	public:
		GameInstance();
		virtual ~GameInstance() noexcept override;

		AFUNCTION()
		virtual std::shared_ptr<Scene> GetEntryScene();
	};
}