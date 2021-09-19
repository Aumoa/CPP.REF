// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngine.h"

class SGameInstance;
class SGameEngine;

class GAME_API SGameModule : virtual public SObject
{
	GENERATED_BODY(SGameModule)

public:
	SGameModule();
	virtual ~SGameModule() override;

	virtual SGameInstance* CreatePrimaryGameModule();
	virtual SGameEngine* CreateGameEngine();

	virtual std::vector<Type*> GetTypeCollection() const;
	virtual Type* GetType(std::wstring_view friendlyName) const;
};

#define DEFINE_GAME_MODULE(GameModuleClass)					\
extern "C"													\
{															\
	__declspec(dllexport)									\
	SGameModule* LoadGameModule()							\
	{														\
		return SObject::NewStaticObject<GameModuleClass>();	\
	}														\
}