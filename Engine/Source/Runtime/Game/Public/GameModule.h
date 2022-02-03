// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SGameInstance;
class SGameEngine;

class GAME_API SGameModule : implements SObject
{
	GENERATED_BODY(SGameModule)

public:
	SGameModule();
	virtual ~SGameModule() override;

	virtual SGameInstance* CreateGameInstance();
	virtual SGameEngine* CreateGameEngine();
};

#define DEFINE_GAME_MODULE(GameModuleClass)	\
extern "C"									\
{											\
	__declspec(dllexport)					\
	SGameModule* LoadGameModule()			\
	{										\
		return gcnew GameModuleClass();		\
	}										\
}