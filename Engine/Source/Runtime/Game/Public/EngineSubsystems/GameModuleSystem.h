// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class SPlatformModule;
class SGameInstance;
class SGameModule;

class GAME_API SGameModuleSystem : public SGameEngineSubsystem
{
	GENERATED_BODY(SGameModuleSystem)

private:
	SPlatformModule* _module = nullptr;
	SGameModule* _gameModule = nullptr;

public:
	SGameModuleSystem();
	virtual ~SGameModuleSystem() override;
	
	void LoadGameModule(std::wstring_view gameModuleName);
	SGameInstance* LoadGameInstance();
};