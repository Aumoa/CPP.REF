// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameModule.h"
#include "GameInstance.h"
#include "GameEngine.h"

GENERATE_BODY(SGameModule);

SGameModule::SGameModule() : Super()
{
}

SGameModule::~SGameModule()
{
}

SGameInstance* SGameModule::CreateGameInstance()
{
	return NewObject<SGameInstance>();
}

SGameEngine* SGameModule::CreateGameEngine()
{
	return NewObject<SGameEngine>();
}

std::vector<Type*> SGameModule::GetTypeCollection() const
{
	return {};
}

Type* SGameModule::GetType(std::wstring_view friendlyName) const
{
	return nullptr;
}

DEFINE_GAME_MODULE(SGameModule);