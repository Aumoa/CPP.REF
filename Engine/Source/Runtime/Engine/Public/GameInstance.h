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
		PPtr<Scene> m_EntryScene;

	public:
		GameInstance();
		virtual ~GameInstance() noexcept override;

		virtual RPtr<Scene> GetEntryScene();
	};
}

#define DEFINE_GAME_INSTANCE_CLASS(Namespace, ClassName) \
extern "C" \
{ \
	PLATFORM_SHARED_EXPORT ::Ayla::RPtr<::Ayla::GameInstance>* CreateGameInstance__() \
	{ \
		return new ::Ayla::RPtr<::Ayla::GameInstance>(::Ayla::Object::New<::Namespace::ClassName>()); \
	} \
}