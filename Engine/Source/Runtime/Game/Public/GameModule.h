// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class GameInstance;

class GAME_API GameModule : virtual public Object
{
	GENERATED_BODY(GameModule)

public:
	GameModule();
	virtual ~GameModule() override;

	virtual GameInstance* CreatePrimaryGameModule() = 0;
};

#define DEFINE_GAME_MODULE(GameModuleClass)		\
extern "C"										\
{												\
	__declspec(dllexport)						\
	GameModule* LoadGameModule()				\
	{											\
		return new GameModuleClass();			\
	}											\
}