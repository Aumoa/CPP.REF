// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameModule.h"
#include "THGameInstance.h"

DEFINE_GAME_MODULE(STHGameModule);

SGameInstance* STHGameModule::CreateGameInstance()
{
	return NewObject<STHGameInstance>();
}